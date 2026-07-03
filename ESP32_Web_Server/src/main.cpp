#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
#include <ArduinoJson.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <ESPmDNS.h>
#include <Preferences.h>
#include "ui.h"
#include "debug.h"



/*********************** 参数定义 ************************/
#define SD_SCK   14
#define SD_MISO  19
#define SD_MOSI  13
#define SD_CS    15

#define MAX_MUSIC_NUM     200
#define MAX_PATH_LEN      70
#define MAX_NAME_LEN      60
#define MAX_EXT_LEN       8
#define API_BUF_SIZE      8192
#define MAX_TRANSMISSION_CAPACITY 20

// 播放列表与最近播放
#define MAX_PLAYLIST_NUM     5
#define MAX_PLAYLIST_SONGS   50
#define MAX_RECENT_SONGS     20

// 文件类型宏
#define TYPE_UNKNOWN  0
#define TYPE_MUSIC    1

// WiFi 配置
#define AP_SSID           "ESP32-Media-Setup"
#define AP_PASSWORD       "12345678"
#define WIFI_RETRY_MAX    1

// AP 默认地址
#define AP_LOCAL_IP       IPAddress(192, 168, 1, 1)
#define AP_GATEWAY_IP     IPAddress(192, 168, 1, 1)
#define AP_SUBNET_MASK    IPAddress(255, 255, 255, 0)

// mDNS 域名
#define MDNS_HOSTNAME     "esp32streaming"

// Preferences 命名空间
#define PREFS_NAMESPACE   "esp32media"



/*********************** 文件信息结构体 ************************/
struct FileInfo {
  char path[MAX_PATH_LEN];
  char name[MAX_NAME_LEN];
  uint8_t type;
  char extention[MAX_EXT_LEN];
  uint32_t size;
};

struct MediaLibrary {
  FileInfo MusicList[MAX_MUSIC_NUM];
  int MusicCount;
  bool isScanned;
};

// 播放列表结构
struct Playlist {
  char name[MAX_NAME_LEN];
  int songs[MAX_PLAYLIST_SONGS];
  int songCount;
};



/*********************** 全局变量 ************************/
SPIClass hspi(HSPI);
MediaLibrary g_mediaLib = {0};
char g_apiBuffer[API_BUF_SIZE];

const char* Music_Ext[] = {".mp3", ".wav", ".flac", ".aac", ".ogg", ".m4a"};
const int   Music_Ext_Num = 6;

const char* Music_MIME[] = {"audio/mpeg","audio/wav","audio/flac","audio/aac","audio/ogg","audio/mp4"};

AsyncWebServer Streaming_WebServer(80);
Preferences g_prefs;

char g_wifiSSID[33] = {0};
char g_wifiPassword[65] = {0};
bool g_wifiConnected = false;

bool SerialDebugEnabled = true;
int g_wifiPower = WIFI_POWER_11dBm;

// 播放列表
Playlist g_playlists[MAX_PLAYLIST_NUM];
int g_playlistCount = 0;

// 最近播放（内存中，重启丢失）
int g_recentSongs[MAX_RECENT_SONGS];
int g_recentCount = 0;

// 主题
char g_theme[32] = "blue";



/*********************** 函数声明 ************************/
// SD卡与文件系统
bool SD_Init();
void MediaLibrary_Scan();
void Recursive_Directory_Scanning(const char* path, int level);
int File_Ext_Checking(const char* filename, const char** extList, int extNum);
void Add_File_To_Library(const char* path, const char* name, uint32_t size, uint8_t type, const char* ext);
void Secure_String_Copying(char* dest, const char* src, int maxLen);
const char* Get_MIME_Type(uint8_t type, const char* ext);

// 播放列表持久化
void Playlist_Load();
void Playlist_Save();

// 最近播放
void Recent_Add(int index);

// Web API
const char* WebAPI_GetStats();
const char* WebAPI_GetMusic_Page(int page, int pageSize);
const char* WebAPI_GetMusicByIndex(int index);
const char* WebAPI_Search(const char* query);
const char* WebAPI_GetPlaylists();
const char* WebAPI_GetPlaylistSongs(int plIndex);
const char* WebAPI_GetRecent();

// 流式传输
void WebAPI_StreamMusic(int index, AsyncWebServerRequest* request);

// HTTP路由
void WebServer_SetupRoutes();

// WiFi管理
void WiFi_LoadSettings();
void WiFi_SaveSettings();
void WiFi_ClearSettings();
bool WiFi_Connect(const char* ssid, const char* password);
void WiFi_StartAP();
void WiFi_Init_MDNS();

// POST处理
void Handle_SaveWiFi(AsyncWebServerRequest* request);
void Handle_ClearWiFi(AsyncWebServerRequest* request);
void Handle_SetDebug(AsyncWebServerRequest* request);
void Handle_SetWiFiPower(AsyncWebServerRequest* request);
void Handle_SetTheme(AsyncWebServerRequest* request);




/*********************** WiFi事件回调 ************************/
void On_WiFi_ClientConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("\n[WiFi事件] 客户端已连接到AP");
  Serial.print("  客户端MAC: ");
  for (int i = 0; i < 6; i++) {
    Serial.printf("%02X", info.wifi_ap_staconnected.mac[i]);
    if (i < 5) Serial.print(":");
  }
  Serial.println();
  Serial.print("  当前连接数: ");
  Serial.println(WiFi.softAPgetStationNum());
}




void On_WiFi_ClientDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.println("\n[WiFi事件] 客户端已断开AP");
  Serial.print("  当前连接数: ");
  Serial.println(WiFi.softAPgetStationNum());
}




void On_WiFi_GotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
  Serial.print("\n[WiFi事件] STA模式获得IP: ");
  Serial.println(WiFi.localIP());
}




/*********************** 字符串操作 ************************/
void Secure_String_Copying(char* dest, const char* src, int maxLen) {
  int i = 0;
  while (src[i] != '\0' && i < maxLen - 1) {
    dest[i] = src[i];
    i++;
  }
  dest[i] = '\0';
}




/*********************** 扩展名检测 ************************/
int File_Ext_Checking(const char* filename, const char** extList, int extNum) {
  int FileNameLen = strlen(filename);

  for (int i = 0; i < extNum; i++) {
    const char* Ext = extList[i];
    int ExtLen = strlen(Ext);

    if (FileNameLen >= ExtLen) {
      const char* ExtName = filename + FileNameLen - ExtLen;
      bool match = true;

      for (int j = 0; j < ExtLen; j++) {
        if (tolower(ExtName[j]) != tolower(Ext[j])) {
          match = false;
          break;
        }
      }
      if (match) {
        return i;
      }
    }
  }
  return -1;
}




/*********************** MIME类型获取 ************************/
const char* Get_MIME_Type(uint8_t type, const char* ext) {
  if (type == TYPE_MUSIC) {
    int idx = File_Ext_Checking(ext, Music_Ext, Music_Ext_Num);
    if (idx >= 0 && idx < Music_Ext_Num) {
      return Music_MIME[idx];
    }
    return "audio/mpeg";
  }
  return "application/octet-stream";
}




/*********************** 文件添加到媒体库 ************************/
void Add_File_To_Library(const char* path, const char* name, uint32_t size, uint8_t type, const char* ext) {
  if (type == TYPE_MUSIC && g_mediaLib.MusicCount < MAX_MUSIC_NUM) {
    FileInfo* info = &g_mediaLib.MusicList[g_mediaLib.MusicCount];
    Secure_String_Copying(info->path, path, MAX_PATH_LEN);
    Secure_String_Copying(info->name, name, MAX_NAME_LEN);
    info->type = type;
    Secure_String_Copying(info->extention, ext, MAX_EXT_LEN);
    info->size = size;
    g_mediaLib.MusicCount++;
  }
}




/*********************** SD卡初始化 ************************/
bool SD_Init() {
  Serial.println("\n》初始化SD卡");

  // 保持旧版可运行程序的初始化方式：
  // 只初始化一次 HSPI，不再反复切换 SPI 频率，也不在失败后反复 SD.end()。
  // 硬件配置为：SCK=14, MISO=19, MOSI=13, CS=15。
  hspi.begin(SD_SCK, SD_MISO, SD_MOSI, SD_CS);

  if (!SD.begin(SD_CS, hspi)) {
    Serial.println("✗ SD卡初始化失败,请检查SD卡读取模块");
    return false;
  }

  uint8_t cardType = SD.cardType();
  uint64_t cardSizeMB = SD.cardSize() / (1024 * 1024);

  Serial.println("√ SD卡初始化成功");
  Serial.print("SD卡类型:");
  Serial.print(cardType == CARD_SD ? "SDSC" : (cardType == CARD_SDHC ? "SDHC" : "其他"));
  Serial.print("，容量");
  Serial.print((uint32_t)cardSizeMB);
  Serial.println("MB");
  return true;
}




/*********************** 递归扫描目录 ************************/
void Recursive_Directory_Scanning(const char* path, int level) {
  if (level > 8) return;

  File root = SD.open(path);
  if (!root || !root.isDirectory()) return;

  File file = root.openNextFile();

  while (file) {
    char fname[MAX_NAME_LEN];
    Secure_String_Copying(fname, file.name(), MAX_NAME_LEN);

    if (strncmp(fname, "System", 6) == 0 ||
        strncmp(fname, "._", 2) == 0 ||
        strncmp(fname, "@", 1) == 0 ||
        fname[0] == '.' ||
        strcmp(fname, "RECYCLED") == 0 ||
        strcmp(fname, "System Volume Information") == 0) {
      file.close();
      file = root.openNextFile();
      continue;
    }

    char fullPath[MAX_PATH_LEN];
    Secure_String_Copying(fullPath, path, MAX_PATH_LEN);
    int pathLen = strlen(fullPath);

    if (pathLen > 1 && fullPath[pathLen-1] != '/') {
      fullPath[pathLen] = '/';
      fullPath[pathLen+1] = '\0';
    }
    strcat(fullPath, fname);

    if (file.isDirectory()) {
      Recursive_Directory_Scanning(fullPath, level + 1);
    } else {
      int musicIdx = File_Ext_Checking(fname, Music_Ext, Music_Ext_Num);

      if (musicIdx >= 0) {
        Add_File_To_Library(fullPath, fname, file.size(), TYPE_MUSIC, Music_Ext[musicIdx]);
      }
    }

    file.close();
    file = root.openNextFile();
  }
  root.close();
}




/*********************** 扫描媒体库（只执行一次） ************************/
void MediaLibrary_Scan() {
  if (g_mediaLib.isScanned) {
    debug_println("媒体库已扫描，跳过");
    return;
  }

  Serial.println("\n》扫描媒体库");

  g_mediaLib.MusicCount = 0;

  Recursive_Directory_Scanning("/", 0);

  g_mediaLib.isScanned = true;

  Serial.print("√ 扫描完成: 音乐 ");
  Serial.print(g_mediaLib.MusicCount);
  Serial.println(" 首");
}




/*********************** 播放列表持久化（JSON → Preferences） ************************/
void Playlist_Load() {
  g_prefs.begin(PREFS_NAMESPACE, true);

  if (g_prefs.isKey("playlists")) {
    String json = g_prefs.getString("playlists", "[]");
    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, json);

    if (!err) {
      JsonArray arr = doc.as<JsonArray>();
      g_playlistCount = 0;

      for (JsonObject pl : arr) {
        if (g_playlistCount >= MAX_PLAYLIST_NUM) break;

        const char* plName = pl["name"];
        Secure_String_Copying(g_playlists[g_playlistCount].name,
                              plName ? plName : "未命名", MAX_NAME_LEN);

        JsonArray songs = pl["songs"];
        int sc = 0;
        for (int s : songs) {
          if (sc >= MAX_PLAYLIST_SONGS) break;
          g_playlists[g_playlistCount].songs[sc++] = s;
        }
        g_playlists[g_playlistCount].songCount = sc;
        g_playlistCount++;
      }
    }
  }

  g_prefs.end();
}




void Playlist_Save() {
  JsonDocument doc;
  JsonArray arr = doc.to<JsonArray>();

  for (int i = 0; i < g_playlistCount; i++) {
    JsonObject pl = arr.add<JsonObject>();
    pl["name"] = g_playlists[i].name;

    JsonArray songs = pl["songs"].to<JsonArray>();
    for (int j = 0; j < g_playlists[i].songCount; j++) {
      songs.add(g_playlists[i].songs[j]);
    }
  }

  String json;
  serializeJson(doc, json);

  g_prefs.begin(PREFS_NAMESPACE, false);
  g_prefs.putString("playlists", json);
  g_prefs.end();
}




/*********************** 最近播放 ************************/
void Recent_Add(int index) {
  // 越界检查
  if (index < 0 || index >= g_mediaLib.MusicCount) return;

  // 移除已存在的相同记录
  for (int i = 0; i < g_recentCount; i++) {
    if (g_recentSongs[i] == index) {
      for (int j = i; j < g_recentCount - 1; j++) {
        g_recentSongs[j] = g_recentSongs[j + 1];
      }
      g_recentCount--;
      break;
    }
  }

  // 插入到最前面
  if (g_recentCount >= MAX_RECENT_SONGS) {
    for (int i = MAX_RECENT_SONGS - 1; i > 0; i--) {
      g_recentSongs[i] = g_recentSongs[i - 1];
    }
    g_recentSongs[0] = index;
  } else {
    for (int i = g_recentCount; i > 0; i--) {
      g_recentSongs[i] = g_recentSongs[i - 1];
    }
    g_recentSongs[0] = index;
    g_recentCount++;
  }
}




/*********************** 辅助：大小写不敏感字符串查找 ************************/
static bool stristr(const char* haystack, const char* needle) {
  if (!haystack || !needle) return false;
  int needleLen = strlen(needle);
  if (needleLen == 0) return true;

  int hayLen = strlen(haystack);
  for (int i = 0; i <= hayLen - needleLen; i++) {
    bool match = true;
    for (int j = 0; j < needleLen; j++) {
      if (tolower(haystack[i + j]) != tolower(needle[j])) {
        match = false;
        break;
      }
    }
    if (match) return true;
  }
  return false;
}




/*********************** Web API实现 ************************/

const char* WebAPI_GetStats() {
  JsonDocument doc;
  doc["status"] = "ok";
  doc["music_count"] = g_mediaLib.MusicCount;
  serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
  return g_apiBuffer;
}




const char* WebAPI_GetMusic_Page(int page, int pageSize) {
  if (pageSize <= 0) pageSize = MAX_TRANSMISSION_CAPACITY;

  // 兼容新版 UI：UI 从 page=1 开始请求；也兼容旧请求 page=0。
  int start = (page <= 0) ? 0 : (page - 1) * pageSize;
  int end = start + pageSize;

  JsonDocument doc;
  doc["status"] = "ok";
  doc["page"] = page;
  doc["page_size"] = pageSize;
  doc["total"] = g_mediaLib.MusicCount;

  // 新版 ui.h 读取 data.files，所以这里使用 files。
  JsonArray files = doc["files"].to<JsonArray>();

  if (start < 0) start = 0;
  if (start < g_mediaLib.MusicCount) {
    if (end > g_mediaLib.MusicCount) end = g_mediaLib.MusicCount;

    for (int i = start; i < end; i++) {
      FileInfo* info = &g_mediaLib.MusicList[i];
      JsonObject song = files.add<JsonObject>();
      song["index"] = i;
      song["name"] = info->name;
      song["path"] = info->path;
      song["size"] = info->size;
      song["size_kb"] = info->size / 1024;
      song["ext"] = info->extention;
    }
  }

  serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
  return g_apiBuffer;
}




const char* WebAPI_GetMusicByIndex(int index) {
  if (index < 0 || index >= g_mediaLib.MusicCount) {
    return "{\"status\":\"error\",\"msg\":\"index out of range\"}";
  }

  JsonDocument doc;
  FileInfo* info = &g_mediaLib.MusicList[index];
  doc["status"] = "ok";
  doc["index"] = index;
  doc["name"] = info->name;
  doc["path"] = info->path;
  doc["size"] = info->size;
  doc["size_kb"] = info->size / 1024;
  doc["ext"] = info->extention;
  doc["mime"] = Get_MIME_Type(TYPE_MUSIC, info->extention);

  serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
  return g_apiBuffer;
}




const char* WebAPI_Search(const char* query) {
  JsonDocument doc;
  doc["status"] = "ok";
  doc["query"] = query;

  JsonArray results = doc["results"].to<JsonArray>();

  if (query && strlen(query) > 0) {
    for (int i = 0; i < g_mediaLib.MusicCount; i++) {
      if (stristr(g_mediaLib.MusicList[i].name, query)) {
        JsonObject song = results.add<JsonObject>();
        song["index"] = i;
        song["name"] = g_mediaLib.MusicList[i].name;
        song["path"] = g_mediaLib.MusicList[i].path;
        song["size"] = g_mediaLib.MusicList[i].size;
        song["size_kb"] = g_mediaLib.MusicList[i].size / 1024;
        song["ext"] = g_mediaLib.MusicList[i].extention;
      }
    }
  }

  doc["count"] = results.size();
  serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
  return g_apiBuffer;
}




const char* WebAPI_GetPlaylists() {
  JsonDocument doc;
  doc["status"] = "ok";

  JsonArray data = doc["playlists"].to<JsonArray>();

  for (int i = 0; i < g_playlistCount; i++) {
    JsonObject pl = data.add<JsonObject>();
    pl["index"] = i;
    pl["name"] = g_playlists[i].name;
    pl["song_count"] = g_playlists[i].songCount;
    pl["count"] = g_playlists[i].songCount;
  }

  doc["total"] = g_playlistCount;
  serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
  return g_apiBuffer;
}




const char* WebAPI_GetPlaylistSongs(int plIndex) {
  if (plIndex < 0 || plIndex >= g_playlistCount) {
    return "{\"status\":\"error\",\"msg\":\"playlist index out of range\"}";
  }

  JsonDocument doc;
  doc["status"] = "ok";
  doc["name"] = g_playlists[plIndex].name;
  doc["index"] = plIndex;

  JsonArray data = doc["songs"].to<JsonArray>();

  for (int i = 0; i < g_playlists[plIndex].songCount; i++) {
    int songIdx = g_playlists[plIndex].songs[i];
    if (songIdx < 0 || songIdx >= g_mediaLib.MusicCount) continue;

    FileInfo* info = &g_mediaLib.MusicList[songIdx];
    JsonObject song = data.add<JsonObject>();
    song["index"] = songIdx;
    song["name"] = info->name;
    song["path"] = info->path;
    song["size"] = info->size;
    song["size_kb"] = info->size / 1024;
    song["ext"] = info->extention;
  }

  serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
  return g_apiBuffer;
}




const char* WebAPI_GetRecent() {
  JsonDocument doc;
  doc["status"] = "ok";

  JsonArray data = doc["recent"].to<JsonArray>();

  for (int i = 0; i < g_recentCount; i++) {
    int songIdx = g_recentSongs[i];
    if (songIdx < 0 || songIdx >= g_mediaLib.MusicCount) continue;

    FileInfo* info = &g_mediaLib.MusicList[songIdx];
    JsonObject song = data.add<JsonObject>();
    song["index"] = songIdx;
    song["name"] = info->name;
    song["path"] = info->path;
    song["size"] = info->size;
    song["size_kb"] = info->size / 1024;
    song["ext"] = info->extention;
  }

  doc["count"] = data.size();
  serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
  return g_apiBuffer;
}




/*********************** 流式传输函数 ************************/
void WebAPI_StreamMusic(int index, AsyncWebServerRequest* request) {
  if (index < 0 || index >= g_mediaLib.MusicCount) {
    request->send(404, "text/plain", "Song index out of range");
    return;
  }

  FileInfo* info = &g_mediaLib.MusicList[index];
  const char* path = info->path;

  if (!SD.exists(path)) {
    request->send(404, "text/plain", "File not found");
    return;
  }

  const char* mimeType = Get_MIME_Type(TYPE_MUSIC, info->extention);
  AsyncWebServerResponse* response = new AsyncFileResponse(SD, path, mimeType, false);
  request->send(response);
}




/*********************** WiFi设置管理（Preferences） ************************/
void WiFi_LoadSettings() {
  g_prefs.begin(PREFS_NAMESPACE, true);

  if (g_prefs.isKey("wifi_ssid")) {
    String ssid = g_prefs.getString("wifi_ssid", "");
    String pass = g_prefs.getString("wifi_pass", "");
    Secure_String_Copying(g_wifiSSID, ssid.c_str(), sizeof(g_wifiSSID));
    Secure_String_Copying(g_wifiPassword, pass.c_str(), sizeof(g_wifiPassword));
  }

  SerialDebugEnabled = g_prefs.getBool("debug_en", true);
  g_wifiPower = g_prefs.getInt("wifi_power", WIFI_POWER_11dBm);

  // 加载主题
  if (g_prefs.isKey("theme")) {
    String theme = g_prefs.getString("theme", "blue");
    Secure_String_Copying(g_theme, theme.c_str(), sizeof(g_theme));
  }

  g_prefs.end();
}




void WiFi_SaveSettings() {
  g_prefs.begin(PREFS_NAMESPACE, false);
  g_prefs.putString("wifi_ssid", g_wifiSSID);
  g_prefs.putString("wifi_pass", g_wifiPassword);
  g_prefs.putBool("debug_en", SerialDebugEnabled);
  g_prefs.putInt("wifi_power", g_wifiPower);
  g_prefs.putString("theme", g_theme);
  g_prefs.end();
}




void WiFi_ClearSettings() {
  g_prefs.begin(PREFS_NAMESPACE, false);
  g_prefs.clear();
  g_prefs.end();
}




/*********************** WiFi连接 ************************/
bool WiFi_Connect(const char* ssid, const char* password) {
  Serial.println("\n========== WiFi连接 ==========");
  Serial.print("SSID: "); Serial.println(ssid);
  Serial.print("密码: "); Serial.println(password);
  Serial.println("正在连接...");

  WiFi.mode(WIFI_STA);
  WiFi.setTxPower((wifi_power_t)g_wifiPower);
  WiFi.setHostname(MDNS_HOSTNAME);
  WiFi.begin(ssid, password);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    delay(500);
    Serial.print(".");
    attempts++;
  }
  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    g_wifiConnected = true;
    Serial.println("√ WiFi连接成功!");
    Serial.print("IP地址: "); Serial.println(WiFi.localIP());
    return true;
  } else {
    g_wifiConnected = false;
    Serial.println("✗ WiFi连接失败!");
    WiFi.disconnect();
    return false;
  }
}




/*********************** WiFi启动AP模式 ************************/
void WiFi_StartAP() {
  Serial.println("\n》启动AP模式");

  WiFi.disconnect(true);
  delay(500);

  WiFi.mode(WIFI_AP);
  WiFi.setTxPower((wifi_power_t)g_wifiPower);

  if (!WiFi.softAPConfig(AP_LOCAL_IP, AP_GATEWAY_IP, AP_SUBNET_MASK)) {
    Serial.println("✗ AP地址配置失败，将继续使用默认地址");
  }

  if (!WiFi.softAP(AP_SSID, AP_PASSWORD)) {
    Serial.println("✗ AP启动失败，5秒后重启");
    delay(5000);
    ESP.restart();
  }

  IPAddress apIP = WiFi.softAPIP();
  Serial.println("√ AP已启动");
  Serial.print("  SSID: "); Serial.println(AP_SSID);
  Serial.print("  密码: "); Serial.println(AP_PASSWORD);
  Serial.print("  IP: "); Serial.println(apIP);
}




/*********************** mDNS启动 ************************/
void WiFi_Init_MDNS() {
  String hostnames[6];
  hostnames[0] = String(MDNS_HOSTNAME);
  hostnames[1] = String(MDNS_HOSTNAME) + "1";
  hostnames[2] = String(MDNS_HOSTNAME) + "2";
  hostnames[3] = String(MDNS_HOSTNAME) + "3";
  hostnames[4] = String(MDNS_HOSTNAME) + "4";
  hostnames[5] = String(MDNS_HOSTNAME) + "5";

  for (int i = 0; i < 6; i++) {
    const char* tryHost = hostnames[i].c_str();

    if (MDNS.begin(tryHost)) {
      Serial.print("》mDNS已启动: http://");
      Serial.print(tryHost);
      Serial.println(".local");

      if (i > 0) {
        debug_print("（域名已被占用，自动分配序号 ");
        debug_print(i);
        debug_println(")");
      }

      MDNS.addService("_http", "_tcp", 80);
      MDNS.addService("_esp32streaming", "_tcp", 80);
      MDNS.addServiceTxt("_http", "_tcp", "path", "/");
      MDNS.addServiceTxt("_http", "_tcp", "name", "ESP32Streaming Music Server");
      return;
    }

    debug_print("域名被占用: "); debug_print(tryHost); debug_println(".local");
    delay(100);
  }

  Serial.println("》mDNS启动失败");
}




/*********************** HTTP服务器路由配置 ************************/
void WebServer_SetupRoutes() {

  // 1. 统计信息
  Streaming_WebServer.on("/api/stats", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "application/json", WebAPI_GetStats());
  });

  // 2. 分页音乐列表
  Streaming_WebServer.on("/api/music", HTTP_GET, [](AsyncWebServerRequest* request) {
    int page = request->hasParam("page") ? request->getParam("page")->value().toInt() : 0;
    int size = request->hasParam("size") ? request->getParam("size")->value().toInt() : MAX_TRANSMISSION_CAPACITY;
    request->send(200, "application/json", WebAPI_GetMusic_Page(page, size));
  });

  // 3. 单首音乐详情
  Streaming_WebServer.on("/api/music/info", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (!request->hasParam("index")) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"missing index\"}");
      return;
    }
    int index = request->getParam("index")->value().toInt();
    request->send(200, "application/json", WebAPI_GetMusicByIndex(index));
  });

  // 4. 音乐流式传输
  Streaming_WebServer.on("/api/stream", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (!request->hasParam("index")) {
      request->send(400, "text/plain", "missing index");
      return;
    }
    int index = request->getParam("index")->value().toInt();
    WebAPI_StreamMusic(index, request);
  });

  // 音乐下载（强制下载而非播放）
  Streaming_WebServer.on("/api/download", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (!request->hasParam("index")) {
      request->send(400, "text/plain", "missing index");
      return;
    }
    int index = request->getParam("index")->value().toInt();
    if (index < 0 || index >= g_mediaLib.MusicCount) {
      request->send(404, "text/plain", "Song index out of range");
      return;
    }
    FileInfo* info = &g_mediaLib.MusicList[index];
    const char* path = info->path;
    if (!SD.exists(path)) {
      request->send(404, "text/plain", "File not found");
      return;
    }
    const char* mimeType = Get_MIME_Type(TYPE_MUSIC, info->extention);
    AsyncWebServerResponse* response = new AsyncFileResponse(SD, path, mimeType, false);
    char disposition[128];
    snprintf(disposition, sizeof(disposition), "attachment; filename=\"%s\"", info->name);
    response->addHeader("Content-Disposition", disposition);
    request->send(response);
  });

  // 5. 搜索
  Streaming_WebServer.on("/api/search", HTTP_GET, [](AsyncWebServerRequest* request) {
    String query = request->hasParam("q") ? request->getParam("q")->value() : "";
    request->send(200, "application/json", WebAPI_Search(query.c_str()));
  });

  // 6. 获取所有播放列表
  Streaming_WebServer.on("/api/playlists", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "application/json", WebAPI_GetPlaylists());
  });

  // 7. 创建播放列表
  Streaming_WebServer.on("/api/playlist/create", HTTP_POST, [](AsyncWebServerRequest* request) {
    if (g_playlistCount >= MAX_PLAYLIST_NUM) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"max playlists reached\"}");
      return;
    }

    String name = request->hasParam("name") ? request->getParam("name")->value() : "未命名";
    Secure_String_Copying(g_playlists[g_playlistCount].name, name.c_str(), MAX_NAME_LEN);
    g_playlists[g_playlistCount].songCount = 0;
    g_playlistCount++;

    Playlist_Save();

    JsonDocument resp;
    resp["status"] = "ok";
    resp["index"] = g_playlistCount - 1;
    resp["name"] = g_playlists[g_playlistCount - 1].name;
    serializeJson(resp, g_apiBuffer, API_BUF_SIZE);
    request->send(200, "application/json", g_apiBuffer);
  });

  // 8. 删除播放列表
  Streaming_WebServer.on("/api/playlist/delete", HTTP_POST, [](AsyncWebServerRequest* request) {
    if (!request->hasParam("index")) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"missing index\"}");
      return;
    }

    int idx = request->getParam("index")->value().toInt();
    if (idx < 0 || idx >= g_playlistCount) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"playlist index out of range\"}");
      return;
    }

    // 左移覆盖
    for (int i = idx; i < g_playlistCount - 1; i++) {
      g_playlists[i] = g_playlists[i + 1];
    }
    g_playlistCount--;

    Playlist_Save();

    request->send(200, "application/json", "{\"status\":\"ok\",\"msg\":\"playlist deleted\"}");
  });

  // 9. 添加歌曲到播放列表
  Streaming_WebServer.on("/api/playlist/add", HTTP_POST, [](AsyncWebServerRequest* request) {
    if (!request->hasParam("pl") || !request->hasParam("song")) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"missing pl or song\"}");
      return;
    }

    int plIdx = request->getParam("pl")->value().toInt();
    int songIdx = request->getParam("song")->value().toInt();

    if (plIdx < 0 || plIdx >= g_playlistCount) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"playlist index out of range\"}");
      return;
    }
    if (songIdx < 0 || songIdx >= g_mediaLib.MusicCount) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"song index out of range\"}");
      return;
    }
    if (g_playlists[plIdx].songCount >= MAX_PLAYLIST_SONGS) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"playlist full\"}");
      return;
    }

    // 检查是否已存在
    for (int i = 0; i < g_playlists[plIdx].songCount; i++) {
      if (g_playlists[plIdx].songs[i] == songIdx) {
        request->send(200, "application/json", "{\"status\":\"ok\",\"msg\":\"song already in playlist\"}");
        return;
      }
    }

    g_playlists[plIdx].songs[g_playlists[plIdx].songCount++] = songIdx;
    Playlist_Save();

    request->send(200, "application/json", "{\"status\":\"ok\",\"msg\":\"song added\"}");
  });

  // 10. 从播放列表移除歌曲
  Streaming_WebServer.on("/api/playlist/remove", HTTP_POST, [](AsyncWebServerRequest* request) {
    if (!request->hasParam("pl") || !request->hasParam("song")) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"missing pl or song\"}");
      return;
    }

    int plIdx = request->getParam("pl")->value().toInt();
    int songIdx = request->getParam("song")->value().toInt();

    if (plIdx < 0 || plIdx >= g_playlistCount) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"playlist index out of range\"}");
      return;
    }

    // 查找并移除
    bool found = false;
    for (int i = 0; i < g_playlists[plIdx].songCount; i++) {
      if (g_playlists[plIdx].songs[i] == songIdx) {
        found = true;
        for (int j = i; j < g_playlists[plIdx].songCount - 1; j++) {
          g_playlists[plIdx].songs[j] = g_playlists[plIdx].songs[j + 1];
        }
        g_playlists[plIdx].songCount--;
        break;
      }
    }

    if (!found) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"song not in playlist\"}");
      return;
    }

    Playlist_Save();
    request->send(200, "application/json", "{\"status\":\"ok\",\"msg\":\"song removed\"}");
  });

  // 11. 获取播放列表歌曲详情
  Streaming_WebServer.on("/api/playlist/songs", HTTP_GET, [](AsyncWebServerRequest* request) {
    if (!request->hasParam("index")) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"missing index\"}");
      return;
    }
    int idx = request->getParam("index")->value().toInt();
    request->send(200, "application/json", WebAPI_GetPlaylistSongs(idx));
  });

  // 12. 获取最近播放
  Streaming_WebServer.on("/api/recent", HTTP_GET, [](AsyncWebServerRequest* request) {
    request->send(200, "application/json", WebAPI_GetRecent());
  });

  // 13. 添加最近播放记录
  Streaming_WebServer.on("/api/recent/add", HTTP_POST, [](AsyncWebServerRequest* request) {
    if (!request->hasParam("index")) {
      request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"missing index\"}");
      return;
    }
    int index = request->getParam("index")->value().toInt();
    Recent_Add(index);
    request->send(200, "application/json", "{\"status\":\"ok\",\"msg\":\"added to recent\"}");
  });

  // 13b. 清空最近播放
  Streaming_WebServer.on("/api/recent/clear", HTTP_POST, [](AsyncWebServerRequest* request) {
    g_recentCount = 0;
    request->send(200, "application/json", "{\"status\":\"ok\",\"msg\":\"recent cleared\"}");
  });

  // 14. 获取主题
  Streaming_WebServer.on("/api/theme", HTTP_GET, [](AsyncWebServerRequest* request) {
    JsonDocument doc;
    doc["status"] = "ok";
    doc["theme"] = g_theme;
    serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
    request->send(200, "application/json", g_apiBuffer);
  });

  // 15. 设置主题
  Streaming_WebServer.on("/api/settings/theme", HTTP_POST, [](AsyncWebServerRequest* request) {
    Handle_SetTheme(request);
  });

  // 16. WiFi扫描
  Streaming_WebServer.on("/api/scanwifi", HTTP_GET, [](AsyncWebServerRequest* request) {
    JsonDocument doc;
    doc["status"] = "ok";

    wifi_mode_t originalMode = WiFi.getMode();
    if (originalMode == WIFI_AP) {
      WiFi.mode(WIFI_AP_STA);
      delay(100);
    }

    int n = WiFi.scanNetworks();
    JsonArray networks = doc["networks"].to<JsonArray>();

    for (int i = 0; i < n; i++) {
      JsonObject net = networks.add<JsonObject>();
      net["ssid"] = WiFi.SSID(i);
      net["rssi"] = WiFi.RSSI(i);
      net["channel"] = WiFi.channel(i);
      net["encrypted"] = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
    }

    WiFi.scanDelete();

    if (originalMode == WIFI_AP) {
      WiFi.mode(WIFI_AP);
    }

    serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
    request->send(200, "application/json", g_apiBuffer);
  });

  // 17. 保存WiFi设置 (POST - form-urlencoded, 需要重启)
  Streaming_WebServer.on("/api/savewifi", HTTP_POST, [](AsyncWebServerRequest* request) {
    Handle_SaveWiFi(request);
  });

  // 18. 清除WiFi设置 (POST, 需要重启)
  Streaming_WebServer.on("/api/clearwifi", HTTP_POST, [](AsyncWebServerRequest* request) {
    Handle_ClearWiFi(request);
  });

  // 19. 设置Debug模式 (POST - form-urlencoded, 即时生效，不重启)
  Streaming_WebServer.on("/api/settings/debug", HTTP_POST, [](AsyncWebServerRequest* request) {
    Handle_SetDebug(request);
  });

  // 20. 设置WiFi功率 (POST - form-urlencoded, 即时生效，不重启)
  Streaming_WebServer.on("/api/settings/wifipower", HTTP_POST, [](AsyncWebServerRequest* request) {
    Handle_SetWiFiPower(request);
  });

  // 21. 获取设置信息
  Streaming_WebServer.on("/api/settings", HTTP_GET, [](AsyncWebServerRequest* request) {
    JsonDocument doc;
    doc["status"] = "ok";
    doc["wifi_ssid"] = g_wifiSSID;
    doc["wifi_connected"] = g_wifiConnected;
    doc["debug_enabled"] = SerialDebugEnabled;
    doc["debug"] = SerialDebugEnabled;
    doc["wifi_power"] = g_wifiPower;
    doc["theme"] = g_theme;

    // AP模式下显示已连接设备数
    if (WiFi.getMode() == WIFI_AP) {
      doc["ap_clients"] = WiFi.softAPgetStationNum();
    } else {
      doc["ap_clients"] = 0;
    }

    if (g_wifiConnected) {
      doc["ip"] = WiFi.localIP().toString();
      doc["hostname"] = "ESP32Streaming";
    } else {
      doc["ip"] = WiFi.softAPIP().toString();
      doc["hostname"] = "ESP32Streaming";
    }

    serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
    request->send(200, "application/json", g_apiBuffer);
  });

  // 本机信息（替代原来的已连接设备）
  Streaming_WebServer.on("/api/device", HTTP_GET, [](AsyncWebServerRequest* request) {
    JsonDocument doc;
    doc["status"] = "ok";
    doc["name"] = "ESP32Streaming";
    if (g_wifiConnected) {
      doc["ip"] = WiFi.localIP().toString();
    } else {
      doc["ip"] = WiFi.softAPIP().toString();
    }
    uint8_t mac[6];
    WiFi.macAddress(mac);
    char macStr[18];
    snprintf(macStr, sizeof(macStr), "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
    doc["mac"] = macStr;
    serializeJson(doc, g_apiBuffer, API_BUF_SIZE);
    request->send(200, "application/json", g_apiBuffer);
  });

  // 22. 404
  Streaming_WebServer.onNotFound([](AsyncWebServerRequest* request) {
    request->send(404, "text/plain", "API Not Found");
  });

  // 23. 主页 UI
  // 使用 beginResponse 直接从 Flash 流式发送，避免把 84KB 的 UI 一次性复制到 RAM。
  Streaming_WebServer.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
    AsyncWebServerResponse* response = request->beginResponse(
      200, "text/html; charset=utf-8",
      (const uint8_t*)UI_HTML, strlen(UI_HTML)
    );
    request->send(response);
  });

  Streaming_WebServer.begin();
  Serial.println("\n》HTTP服务器已启动");
}




/*********************** POST请求处理函数 ************************/
void Handle_SaveWiFi(AsyncWebServerRequest* request) {
  String ssid = "";
  String password = "";

  if (request->hasParam("ssid", true)) {
    ssid = request->getParam("ssid", true)->value();
  }
  if (request->hasParam("password", true)) {
    password = request->getParam("password", true)->value();
  }

  if (ssid.length() == 0) {
    request->send(400, "application/json", "{\"status\":\"error\",\"msg\":\"ssid required\"}");
    return;
  }

  Serial.println("\n[设置] 保存WiFi配置:");
  Serial.print("  SSID: "); Serial.println(ssid);
  Serial.print("  密码: "); Serial.println(password);

  Secure_String_Copying(g_wifiSSID, ssid.c_str(), sizeof(g_wifiSSID));
  Secure_String_Copying(g_wifiPassword, password.c_str(), sizeof(g_wifiPassword));

  WiFi_SaveSettings();

  request->send(200, "application/json", "{\"status\":\"ok\",\"msg\":\"saved, rebooting...\"}");

  delay(1000);
  ESP.restart();
}




void Handle_ClearWiFi(AsyncWebServerRequest* request) {
  Serial.println("\n[设置] 清除WiFi配置");

  WiFi_ClearSettings();
  g_wifiSSID[0] = '\0';
  g_wifiPassword[0] = '\0';

  request->send(200, "application/json", "{\"status\":\"ok\",\"msg\":\"wifi cleared, rebooting...\"}");

  delay(1000);
  ESP.restart();
}




void Handle_SetDebug(AsyncWebServerRequest* request) {
  String enabledStr = "0";
  if (request->hasParam("enabled", true)) {
    enabledStr = request->getParam("enabled", true)->value();
  } else if (request->hasParam("enabled")) {
    enabledStr = request->getParam("enabled")->value();
  }

  bool newState = (enabledStr == "1" || enabledStr == "true");
  Serial.print("\n[设置] Debug模式: ");
  Serial.println(newState ? "开启" : "关闭");

  SerialDebugEnabled = newState;
  WiFi_SaveSettings();

  JsonDocument resp;
  resp["status"] = "ok";
  resp["debug_enabled"] = SerialDebugEnabled;
  resp["debug"] = SerialDebugEnabled;
  serializeJson(resp, g_apiBuffer, API_BUF_SIZE);
  request->send(200, "application/json", g_apiBuffer);
}




void Handle_SetWiFiPower(AsyncWebServerRequest* request) {
  String powerStr = "44";
  if (request->hasParam("power", true)) {
    powerStr = request->getParam("power", true)->value();
  } else if (request->hasParam("power")) {
    powerStr = request->getParam("power")->value();
  }

  int newPower = powerStr.toInt();
  Serial.print("\n[设置] WiFi功率: ");
  Serial.println(newPower);

  g_wifiPower = newPower;
  WiFi.setTxPower((wifi_power_t)g_wifiPower);
  WiFi_SaveSettings();

  JsonDocument resp;
  resp["status"] = "ok";
  resp["wifi_power"] = g_wifiPower;
  serializeJson(resp, g_apiBuffer, API_BUF_SIZE);
  request->send(200, "application/json", g_apiBuffer);
}




void Handle_SetTheme(AsyncWebServerRequest* request) {
  String theme = request->hasParam("theme") ? request->getParam("theme")->value() : "blue";

  Secure_String_Copying(g_theme, theme.c_str(), sizeof(g_theme));

  // 保存到 Preferences
  g_prefs.begin(PREFS_NAMESPACE, false);
  g_prefs.putString("theme", g_theme);
  g_prefs.end();

  JsonDocument resp;
  resp["status"] = "ok";
  resp["theme"] = g_theme;
  serializeJson(resp, g_apiBuffer, API_BUF_SIZE);
  request->send(200, "application/json", g_apiBuffer);
}




/*********************** 主程序 ************************/

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println("\n========================================");
  Serial.println("  ESP32Streaming 音乐服务器");
  Serial.println("========================================");

  // 注册WiFi事件
  WiFi.onEvent(On_WiFi_ClientConnected, ARDUINO_EVENT_WIFI_AP_STACONNECTED);
  WiFi.onEvent(On_WiFi_ClientDisconnected, ARDUINO_EVENT_WIFI_AP_STADISCONNECTED);
  WiFi.onEvent(On_WiFi_GotIP, ARDUINO_EVENT_WIFI_STA_GOT_IP);

  // 加载设置
  WiFi_LoadSettings();

  // 加载播放列表
  Playlist_Load();

  // 初始化SD卡
  if (!SD_Init()) {
    Serial.println("系统停止");
    while (1) delay(1000);
  }

  // 扫描媒体库
  MediaLibrary_Scan();

  // WiFi连接逻辑（先尝试连接已保存的WiFi）
  bool wifiSuccess = false;
  if (strlen(g_wifiSSID) > 0) {
    Serial.println("\n》尝试连接已保存的WiFi...");
    wifiSuccess = WiFi_Connect(g_wifiSSID, g_wifiPassword);
  }
  if (!wifiSuccess) {
    WiFi_StartAP();
  }

  // 启动mDNS
  WiFi_Init_MDNS();

  // 启动Web服务器
  WebServer_SetupRoutes();

  // 系统就绪信息
  Serial.println("\n》系统就绪");
  if (wifiSuccess) {
    Serial.print("已连接 WiFi: "); Serial.println(g_wifiSSID);
    Serial.print("访问: http://"); Serial.println(WiFi.localIP().toString());
  } else {
    Serial.print("请连接热点: "); Serial.println(AP_SSID);
    Serial.print("密码: "); Serial.println(AP_PASSWORD);
    Serial.print("访问: http://"); Serial.println(WiFi.softAPIP().toString());
  }
  Serial.println("或: http://esp32streaming.local");
}


void loop() {
  // 异步服务器，无需处理HTTP请求
  delay(1000);
}
