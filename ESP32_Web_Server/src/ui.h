#ifndef UI_H
#define UI_H

const char* UI_HTML = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width,initial-scale=1.0,maximum-scale=1.0,user-scalable=no,viewport-fit=cover">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-status-bar-style" content="default">
<meta name="theme-color" content="#7B9AAE">
<title>ESP32Streaming</title>
<style>
:root{
--primary:#7B9AAE;--primary-dark:#5E8698;--primary-light:#A8C4D4;--primary-ultra-light:#E8EFF4;
--bg:#F4F7FA;--card:#FFFFFF;--card-secondary:#EDF2F7;
--text-primary:#2C3E50;--text-secondary:#5D7085;--text-tertiary:#8FA4B8;
--divider:#E4EAF0;--player-bg:rgba(255,255,255,0.85);
--hero-start:#7B9AAE;--hero-end:#A8C4D4;
--tab-active-bg:var(--primary-ultra-light);
--toggle-bg:#CDD9E6;
}
.theme-green{
--primary:#8AAD93;--primary-dark:#6E947A;--primary-light:#B3CDB8;--primary-ultra-light:#E5F0E8;
--bg:#F5F8F6;--card:#FFFFFF;--card-secondary:#F0F4F1;
--text-primary:#2D3B35;--text-secondary:#6B7F73;--text-tertiary:#9AADA1;
--divider:#E8EDE9;--player-bg:rgba(255,255,255,0.85);
--hero-start:#8AAD93;--hero-end:#B3CDB8;
--tab-active-bg:var(--primary-ultra-light);
--toggle-bg:#C8D8CB;
}
.theme-pink{
--primary:#C4A0A0;--primary-dark:#A88484;--primary-light:#D8BFBF;--primary-ultra-light:#F2E8E8;
--bg:#FAF5F5;--card:#FFFFFF;--card-secondary:#F8F0F0;
--text-primary:#3D2C2C;--text-secondary:#7A6565;--text-tertiary:#B09898;
--divider:#EBE0E0;--player-bg:rgba(255,255,255,0.85);
--hero-start:#C4A0A0;--hero-end:#D8BFBF;
--tab-active-bg:var(--primary-ultra-light);
--toggle-bg:#DDC8C8;
}

*,*::before,*::after{box-sizing:border-box;margin:0;padding:0;}
html,body{height:100%;overflow:hidden;-webkit-tap-highlight-color:transparent;}
body{
font-family:-apple-system,BlinkMacSystemFont,'Segoe UI',Roboto,Helvetica,Arial,sans-serif;
background:var(--bg);color:var(--text-primary);
-webkit-font-smoothing:antialiased;-moz-osx-font-smoothing:grayscale;
user-select:none;-webkit-user-select:none;
display:flex;flex-direction:column;height:100vh;height:100dvh;
overflow:hidden;
}
::-webkit-scrollbar{display:none;width:0;height:0;}
*{scrollbar-width:none;-ms-overflow-style:none;}

.app{flex:1;display:flex;flex-direction:column;overflow:hidden;position:relative;}

/* Top Bar */
.top-bar{
display:flex;align-items:center;justify-content:space-between;
padding:12px 20px;padding-top:max(12px,env(safe-area-inset-top));
flex-shrink:0;z-index:10;
}
.top-bar-title{font-size:22px;font-weight:700;letter-spacing:-0.3px;color:var(--text-primary);}
.top-bar-btn{
width:36px;height:36px;border-radius:50%;border:none;background:var(--card-secondary);
display:flex;align-items:center;justify-content:center;cursor:pointer;
transition:transform 0.15s,background 0.2s;color:var(--text-secondary);
}
.top-bar-btn:active{transform:scale(0.92);background:var(--divider);}

/* Tab Nav */
.tab-nav{
display:flex;padding:0 20px;gap:4px;flex-shrink:0;margin-bottom:8px;
}
.tab-btn{
flex:1;padding:10px 0;border:none;background:transparent;color:var(--text-tertiary);
font-size:15px;font-weight:500;cursor:pointer;border-radius:10px;
transition:color 0.2s,background 0.2s;position:relative;
}
.tab-btn.active{color:var(--primary-dark);background:var(--tab-active-bg);font-weight:600;}
.tab-btn:active{background:var(--divider);}

/* Tab Content */
.tab-content{flex:1;overflow-y:auto;overflow-x:hidden;padding-bottom:90px;}
.tab-panel{display:none;padding:0 20px;}
.tab-panel.active{display:block;}

/* Hero */
.hero-section{
background:linear-gradient(135deg,var(--hero-start),var(--hero-end));
border-radius:16px;padding:24px;margin-bottom:20px;color:#fff;position:relative;overflow:hidden;
}
.hero-section::after{
content:'';position:absolute;top:-40px;right:-40px;width:140px;height:140px;
border-radius:50%;background:rgba(255,255,255,0.1);
}
.hero-greeting{font-size:26px;font-weight:700;margin-bottom:4px;position:relative;z-index:1;}
.hero-subtitle{font-size:14px;opacity:0.85;position:relative;z-index:1;}

/* Section */
.section{margin-bottom:20px;}
.section-header{display:flex;align-items:center;justify-content:space-between;margin-bottom:12px;}
.section-title{font-size:18px;font-weight:700;color:var(--text-primary);}
.section-more{font-size:13px;color:var(--text-tertiary);cursor:pointer;}

/* Horizontal Scroll */
.hscroll{display:flex;gap:12px;overflow-x:auto;padding-bottom:4px;scroll-snap-type:x mandatory;}
.hscroll>*{scroll-snap-align:start;flex-shrink:0;}

/* Playlist Card */
.pl-card{
width:150px;border-radius:14px;overflow:hidden;background:var(--card);
box-shadow:0 2px 8px rgba(0,0,0,0.06);cursor:pointer;transition:transform 0.2s,box-shadow 0.2s;
}
.pl-card:active{transform:scale(0.96);}
.pl-card-cover{
height:120px;background:linear-gradient(135deg,var(--primary-light),var(--primary-dark));
display:flex;align-items:center;justify-content:center;position:relative;
}
.pl-card-cover svg{width:40px;height:40px;opacity:0.5;color:#fff;}
.pl-card-info{padding:10px 12px;}
.pl-card-name{font-size:14px;font-weight:600;color:var(--text-primary);white-space:nowrap;overflow:hidden;text-overflow:ellipsis;}
.pl-card-count{font-size:12px;color:var(--text-tertiary);margin-top:2px;}

.pl-card-create .pl-card-cover{
background:var(--card-secondary);border:2px dashed var(--divider);border-radius:14px 14px 0 0;
}
.pl-card-create .pl-card-cover svg{color:var(--text-tertiary);opacity:0.7;}

/* Song Card */
.song-card{
width:150px;border-radius:14px;overflow:hidden;background:var(--card);
box-shadow:0 2px 8px rgba(0,0,0,0.06);cursor:pointer;transition:transform 0.2s;
}
.song-card:active{transform:scale(0.96);}
.song-card-cover{
height:120px;display:flex;align-items:center;justify-content:center;
}
.song-card-cover .eq-bars{display:none;}
.song-card.playing .song-card-cover .eq-bars{display:flex;}
.song-card.playing .song-card-cover .cover-placeholder{display:none;}

.cover-placeholder{width:50px;height:50px;border-radius:12px;opacity:0.5;}
.song-card-info{padding:10px 12px;}
.song-card-name{font-size:13px;font-weight:600;color:var(--text-primary);white-space:nowrap;overflow:hidden;text-overflow:ellipsis;}
.song-card-artist{font-size:11px;color:var(--text-tertiary);margin-top:2px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis;}

/* Equalizer Bars */
.eq-bars{display:flex;align-items:flex-end;gap:3px;height:30px;}
.eq-bar{width:3px;border-radius:2px;background:var(--primary);animation:eqAnim 1.2s ease-in-out infinite;}
.eq-bar:nth-child(1){height:12px;animation-delay:0s;}
.eq-bar:nth-child(2){height:20px;animation-delay:0.2s;}
.eq-bar:nth-child(3){height:8px;animation-delay:0.4s;}
.eq-bar:nth-child(4){height:16px;animation-delay:0.6s;}
@keyframes eqAnim{0%,100%{transform:scaleY(0.4);}50%{transform:scaleY(1);}}

/* Music List */
.music-list{display:flex;flex-direction:column;gap:6px;}
.music-item{
display:flex;align-items:center;gap:12px;padding:10px 12px;border-radius:12px;
background:var(--card);cursor:pointer;transition:background 0.15s,transform 0.15s;
}
.music-item:active{background:var(--card-secondary);transform:scale(0.985);}
.music-item.playing{background:var(--primary-ultra-light);}
.music-cover{
width:46px;height:46px;border-radius:10px;flex-shrink:0;
display:flex;align-items:center;justify-content:center;
}
.music-cover .eq-bars{display:none;}
.music-item.playing .music-cover .eq-bars{display:flex;}
.music-item.playing .music-cover .cover-placeholder-sm{display:none;}
.cover-placeholder-sm{width:28px;height:28px;border-radius:8px;opacity:0.5;}
.music-info{flex:1;min-width:0;}
.music-name{font-size:14px;font-weight:600;color:var(--text-primary);white-space:nowrap;overflow:hidden;text-overflow:ellipsis;}
.music-meta{font-size:12px;color:var(--text-tertiary);margin-top:2px;}
.music-add-btn{
width:32px;height:32px;border-radius:50%;border:none;background:var(--card-secondary);
display:flex;align-items:center;justify-content:center;cursor:pointer;flex-shrink:0;
color:var(--text-secondary);transition:transform 0.15s,background 0.2s;
}
.music-add-btn:active{transform:scale(0.85);background:var(--divider);}

/* Search */
.search-container{position:relative;margin-bottom:16px;}
.search-input-wrap{
display:flex;align-items:center;gap:10px;background:var(--card);border-radius:14px;
padding:12px 16px;box-shadow:0 2px 8px rgba(0,0,0,0.05);transition:box-shadow 0.2s;
}
.search-input-wrap:focus-within{box-shadow:0 2px 16px rgba(0,0,0,0.1);}
.search-icon{color:var(--text-tertiary);flex-shrink:0;}
.search-input{
flex:1;border:none;outline:none;background:transparent;font-size:16px;color:var(--text-primary);
}
.search-input::placeholder{color:var(--text-tertiary);}
.search-clear{
width:28px;height:28px;border-radius:50%;border:none;background:var(--card-secondary);
display:none;align-items:center;justify-content:center;cursor:pointer;color:var(--text-tertiary);
}
.search-input:not(:placeholder-shown)~.search-clear{display:flex;}

.search-history{margin-bottom:16px;}
.search-history-title{font-size:14px;font-weight:600;color:var(--text-secondary);margin-bottom:8px;}
.search-history-item{
padding:10px 12px;border-radius:10px;cursor:pointer;color:var(--text-secondary);
font-size:14px;transition:background 0.15s;
}
.search-history-item:active{background:var(--card-secondary);}
.search-history-empty{color:var(--text-tertiary);font-size:13px;text-align:center;padding:20px 0;}

.search-results{display:flex;flex-direction:column;gap:6px;}
.search-result-item{
display:flex;align-items:center;gap:12px;padding:10px 12px;border-radius:12px;
background:var(--card);cursor:pointer;transition:background 0.15s;
}
.search-result-item:active{background:var(--card-secondary);}
.search-result-cover{
width:46px;height:46px;border-radius:10px;flex-shrink:0;
background:linear-gradient(135deg,var(--primary-light),var(--primary-dark));
}
.search-result-info{flex:1;min-width:0;}
.search-result-name{font-size:14px;font-weight:600;color:var(--text-primary);white-space:nowrap;overflow:hidden;text-overflow:ellipsis;}
.search-result-meta{font-size:12px;color:var(--text-tertiary);margin-top:2px;}

/* Settings */
.settings-section{margin-bottom:24px;}
.settings-section-title{font-size:16px;font-weight:700;color:var(--text-primary);margin-bottom:12px;}

.theme-cards{display:flex;gap:10px;overflow-x:auto;padding-bottom:4px;}
.theme-card{
flex:1;min-width:100px;padding:16px 12px;border-radius:14px;border:2px solid var(--divider);
background:var(--card);cursor:pointer;text-align:center;transition:border-color 0.2s,transform 0.15s,box-shadow 0.2s;
}
.theme-card:active{transform:scale(0.96);}
.theme-card.active{border-color:var(--primary);box-shadow:0 0 0 3px var(--primary-ultra-light);}
.theme-card-dot{
width:36px;height:36px;border-radius:50%;margin:0 auto 8px;
}
.theme-card-dot.blue{background:linear-gradient(135deg,#7B9AAE,#A8C4D4);}
.theme-card-dot.green{background:linear-gradient(135deg,#8AAD93,#B3CDB8);}
.theme-card-dot.pink{background:linear-gradient(135deg,#C4A0A0,#D8BFBF);}
.theme-card-label{font-size:13px;font-weight:600;color:var(--text-primary);}
.theme-card-check{color:var(--primary);font-size:20px;margin-top:4px;display:none;}
.theme-card.active .theme-card-check{display:block;}

.language-cards{display:flex;gap:10px;}
.language-card{
flex:1;padding:14px 12px;border-radius:14px;border:2px solid var(--divider);
background:var(--card);cursor:pointer;text-align:center;transition:border-color 0.2s,transform 0.15s,box-shadow 0.2s;
}
.language-card:active{transform:scale(0.96);}
.language-card.active{border-color:var(--primary);box-shadow:0 0 0 3px var(--primary-ultra-light);}
.language-card-title{font-size:14px;font-weight:700;color:var(--text-primary);}
.language-card-sub{font-size:11px;color:var(--text-tertiary);margin-top:2px;}

.settings-row{display:flex;align-items:center;justify-content:space-between;padding:12px 0;}
.settings-row-label{font-size:14px;color:var(--text-primary);}
.settings-row-text{display:flex;flex-direction:column;gap:3px;min-width:0;padding-right:12px;}
.settings-row-desc{font-size:12px;line-height:1.35;color:var(--text-tertiary);}
.settings-btn{
padding:10px 18px;border-radius:10px;border:none;background:var(--primary);color:#fff;
font-size:14px;font-weight:600;cursor:pointer;transition:transform 0.15s,opacity 0.2s;
}
.settings-btn:active{transform:scale(0.95);opacity:0.85;}
.settings-btn.secondary{background:var(--card-secondary);color:var(--text-primary);}
.settings-btn.danger{background:var(--primary-dark);color:#fff;}

.settings-input{
width:100%;padding:12px 14px;border-radius:10px;border:1.5px solid var(--divider);
background:var(--card);font-size:14px;color:var(--text-primary);outline:none;
transition:border-color 0.2s;margin-bottom:8px;
}
.settings-input:focus{border-color:var(--primary);}

.settings-select{
padding:10px 14px;border-radius:10px;border:1.5px solid var(--divider);
background:var(--card);font-size:14px;color:var(--text-primary);outline:none;
transition:border-color 0.2s;
}
.settings-select:focus{border-color:var(--primary);}

/* Toggle */
.toggle{position:relative;width:50px;height:28px;flex-shrink:0;}
.toggle input{display:none;}
.toggle-slider{
position:absolute;inset:0;border-radius:14px;background:var(--toggle-bg);
cursor:pointer;transition:background 0.3s;
}
.toggle-slider::before{
content:'';position:absolute;top:3px;left:3px;width:22px;height:22px;
border-radius:50%;background:#fff;transition:transform 0.3s;box-shadow:0 1px 3px rgba(0,0,0,0.15);
}
.toggle input:checked+.toggle-slider{background:var(--primary);}
.toggle input:checked+.toggle-slider::before{transform:translateX(22px);}

/* WiFi List */
.wifi-list{display:flex;flex-direction:column;gap:6px;margin-bottom:12px;max-height:200px;overflow-y:auto;}
.wifi-item{
display:flex;align-items:center;gap:10px;padding:10px 12px;border-radius:10px;
background:var(--card-secondary);cursor:pointer;transition:background 0.15s;
}
.wifi-item:active{background:var(--divider);}
.wifi-signal{color:var(--text-tertiary);flex-shrink:0;}
.wifi-ssid{flex:1;font-size:14px;font-weight:500;color:var(--text-primary);}
.wifi-strength{font-size:11px;color:var(--text-tertiary);}

/* Device List */
.device-item{
display:flex;align-items:center;gap:12px;padding:10px 12px;border-radius:10px;
background:var(--card-secondary);margin-bottom:6px;
}
.device-icon{color:var(--primary);flex-shrink:0;}
.device-info{flex:1;}
.device-name{font-size:14px;font-weight:600;color:var(--text-primary);}
.device-meta{font-size:12px;color:var(--text-tertiary);}

/* Server Info */
.server-info-card{
background:var(--card);border-radius:14px;padding:14px 16px;box-shadow:0 2px 8px rgba(0,0,0,0.04);
}
.server-info-row{display:flex;justify-content:space-between;padding:8px 0;font-size:13px;}
.server-info-row:not(:last-child){border-bottom:1px solid var(--divider);}
.server-info-label{color:var(--text-tertiary);}
.server-info-value{color:var(--text-primary);font-weight:500;}

/* Mini Player */
.mini-player{
position:fixed;bottom:0;left:0;right:0;z-index:100;
background:linear-gradient(135deg,var(--primary),var(--primary-dark));
color:#fff;backdrop-filter:blur(20px);-webkit-backdrop-filter:blur(20px);
border-top:1px solid rgba(255,255,255,0.18);
padding-bottom:max(8px,env(safe-area-inset-bottom));
box-shadow:0 -8px 28px rgba(0,0,0,0.12);
}
.mini-player::before{
content:"";position:absolute;left:18px;right:18px;top:0;height:2px;
background:linear-gradient(90deg,transparent,rgba(255,255,255,0.75),transparent);
animation:miniPulse 2.4s ease-in-out infinite;opacity:0.7;
}
@keyframes miniPulse{0%,100%{transform:scaleX(0.35);opacity:0.25;}50%{transform:scaleX(1);opacity:0.85;}}
.mini-player-progress{height:2px;background:rgba(255,255,255,0.22);position:relative;}
.mini-player-progress-fill{height:100%;background:#fff;width:0%;transition:width 0.3s linear;}
.mini-player-main{display:flex;align-items:center;gap:12px;padding:10px 16px;cursor:pointer;}
.mini-player-btn{
width:36px;height:36px;border-radius:50%;border:none;background:transparent;
display:flex;align-items:center;justify-content:center;cursor:pointer;color:#fff;
flex-shrink:0;
}
.mini-player-btn:active{background:rgba(255,255,255,0.18);}
.mini-player-info{flex:1;min-width:0;overflow:hidden;}
.mini-player-song{
font-size:14px;font-weight:600;color:#fff;
white-space:nowrap;overflow:hidden;text-overflow:ellipsis;
}
.mini-player-song.scroll{animation:marquee 8s linear infinite;}
@keyframes marquee{0%{transform:translateX(0);}100%{transform:translateX(-50%);}}
.mini-player-eq{display:none;align-items:flex-end;gap:3px;height:14px;margin-top:4px;}
.mini-player-eq.active{display:flex;}
.mini-eq-bar{width:3px;border-radius:2px;background:rgba(255,255,255,0.7);animation:miniEqBar 0.8s ease-in-out infinite;}
.mini-eq-bar:nth-child(1){height:6px;animation-delay:0s;}
.mini-eq-bar:nth-child(2){height:10px;animation-delay:0.15s;}
.mini-eq-bar:nth-child(3){height:14px;animation-delay:0.3s;}
.mini-eq-bar:nth-child(4){height:8px;animation-delay:0.45s;}
.mini-eq-bar:nth-child(5){height:12px;animation-delay:0.6s;}
@keyframes miniEqBar{0%,100%{transform:scaleY(0.4);opacity:0.5;}50%{transform:scaleY(1);opacity:1;}}
.mini-player-disc{width:36px;height:36px;border-radius:50%;display:flex;align-items:center;justify-content:center;color:rgba(255,255,255,0.7);flex-shrink:0;opacity:0.5;transition:opacity 0.3s;}
.mini-player-disc.active{opacity:1;animation:discSpin 3s linear infinite;}
@keyframes discSpin{from{transform:rotate(0deg);}to{transform:rotate(360deg);}}

/* Full Player */
.full-player{
position:fixed;inset:0;z-index:200;background:var(--bg);
display:none;flex-direction:column;padding:20px;
padding-top:max(20px,env(safe-area-inset-top));
padding-bottom:max(20px,env(safe-area-inset-bottom));
}
.full-player.active{display:flex;}
.full-player-back{
width:40px;height:40px;border-radius:50%;border:none;background:var(--card-secondary);
display:flex;align-items:center;justify-content:center;cursor:pointer;color:var(--text-secondary);
transition:transform 0.15s;flex-shrink:0;
}
.full-player-back:active{transform:scale(0.9);}
.full-player-cover-area{
flex:1;display:flex;align-items:center;justify-content:center;min-height:0;
}
.full-player-cover{
  width:min(280px,60vw);height:min(280px,60vw);border-radius:50%;
  background:linear-gradient(135deg,var(--primary-light),var(--primary-dark));
  display:flex;align-items:center;justify-content:center;box-shadow:0 20px 60px rgba(0,0,0,0.15);
  transition:transform 0.3s;
}
.full-player-cover.playing{
  animation:disc-spin 8s linear infinite;
}
@keyframes disc-spin{
  from{transform:rotate(0deg);}
  to{transform:rotate(360deg);}
}
.full-player-cover .eq-bars{display:none;gap:5px;height:50px;}
.full-player-cover.playing .eq-bars{display:none;}
.full-player-cover.playing .cover-placeholder-fp{display:flex;align-items:center;justify-content:center;}
.cover-placeholder-fp{width:64px;height:64px;border-radius:50%;opacity:0.4;display:flex;align-items:center;justify-content:center;}
.full-player-info-area{text-align:center;padding:20px 0;}
.full-player-song-name{
font-size:22px;font-weight:700;color:var(--text-primary);
white-space:nowrap;overflow:hidden;text-overflow:ellipsis;display:block;
}
.full-player-song-name.scroll{animation:marquee 8s linear infinite;}
.full-player-song-info{font-size:13px;color:var(--text-tertiary);margin-top:6px;}

/* Progress */
.progress-container{display:flex;align-items:center;gap:10px;padding:0 8px;}
.progress-time{font-size:11px;color:var(--text-tertiary);width:40px;text-align:center;flex-shrink:0;}
.progress-bar-wrap{flex:1;height:6px;position:relative;cursor:pointer;}
.progress-bar-bg{width:100%;height:100%;border-radius:3px;background:var(--divider);position:absolute;}
.progress-bar-fill{height:100%;border-radius:3px;background:var(--primary);position:absolute;width:0%;}
.progress-bar-thumb{
width:14px;height:14px;border-radius:50%;background:var(--primary);position:absolute;
top:50%;transform:translate(-50%,-50%);left:0%;display:none;box-shadow:0 2px 6px rgba(0,0,0,0.2);
}
.progress-bar-wrap:active .progress-bar-thumb,.progress-bar-wrap:hover .progress-bar-thumb{display:block;}

/* Controls */
.controls-row{display:flex;align-items:center;justify-content:center;gap:20px;padding:10px 0;}
.ctrl-btn{
width:44px;height:44px;border-radius:50%;border:none;background:transparent;
display:flex;align-items:center;justify-content:center;cursor:pointer;color:var(--text-primary);
transition:transform 0.15s,background 0.2s;
}
.ctrl-btn:active{transform:scale(0.88);background:var(--divider);}
.ctrl-btn.primary{
width:56px;height:56px;background:var(--primary);color:#fff;box-shadow:0 4px 16px rgba(0,0,0,0.15);
}
.ctrl-btn.primary:active{transform:scale(0.9);opacity:0.9;}
.ctrl-btn.small{width:36px;height:36px;}
.ctrl-btn.mode{color:var(--text-tertiary);font-size:12px;font-weight:600;width:auto;border-radius:8px;padding:0 8px;}

/* Volume */
.volume-row{display:flex;align-items:center;gap:10px;padding:8px 20px;justify-content:center;}
.volume-icon{color:var(--text-tertiary);flex-shrink:0;}
.volume-slider-wrap{flex:1;max-width:200px;height:4px;position:relative;cursor:pointer;}
.volume-slider-bg{width:100%;height:100%;border-radius:2px;background:var(--divider);position:absolute;}
.volume-slider-fill{height:100%;border-radius:2px;background:var(--primary);position:absolute;}

/* Playlist Modal */
.modal-overlay{
position:fixed;inset:0;z-index:300;background:rgba(0,0,0,0.4);
display:none;align-items:flex-end;justify-content:center;
}
.modal-overlay.active{display:flex;}
.modal-sheet{
width:100%;max-width:500px;background:var(--bg);border-radius:20px 20px 0 0;
padding:20px;max-height:60vh;overflow-y:auto;animation:slideUp 0.3s ease;
}
@keyframes slideUp{from{transform:translateY(100%);}to{transform:translateY(0);}}
.modal-sheet-header{display:flex;align-items:center;justify-content:space-between;margin-bottom:16px;}
.modal-sheet-title{font-size:18px;font-weight:700;color:var(--text-primary);}
.modal-close-btn{
width:32px;height:32px;border-radius:50%;border:none;background:var(--card-secondary);
display:flex;align-items:center;justify-content:center;cursor:pointer;color:var(--text-secondary);
}
.modal-playlist-item{
display:flex;align-items:center;gap:12px;padding:12px 0;border-bottom:1px solid var(--divider);
}
.modal-playlist-name{flex:1;font-size:15px;font-weight:500;color:var(--text-primary);}
.modal-playlist-toggle{
width:28px;height:28px;border-radius:50%;border:2px solid var(--divider);
background:transparent;cursor:pointer;flex-shrink:0;transition:all 0.2s;
display:flex;align-items:center;justify-content:center;
}
.modal-playlist-toggle.active{background:var(--primary);border-color:var(--primary);}
.modal-playlist-toggle:active{transform:scale(0.85);}

/* Rename Dialog */
.rename-overlay{
position:fixed;inset:0;z-index:400;background:rgba(0,0,0,0.5);
display:none;align-items:center;justify-content:center;
}
.rename-overlay.active{display:flex;}
.rename-dialog{
background:var(--card);border-radius:16px;padding:20px;width:280px;text-align:center;
}
.rename-dialog-title{font-size:16px;font-weight:700;margin-bottom:12px;color:var(--text-primary);}
.rename-dialog input{
width:100%;padding:10px 12px;border-radius:10px;border:1.5px solid var(--divider);
font-size:14px;outline:none;margin-bottom:12px;background:var(--bg);color:var(--text-primary);
}
.rename-dialog input:focus{border-color:var(--primary);}
.rename-dialog-btns{display:flex;gap:8px;}
.rename-dialog-btns button{
flex:1;padding:10px;border-radius:10px;border:none;font-size:14px;font-weight:600;cursor:pointer;
}
.rename-dialog-btns .btn-cancel{background:var(--card-secondary);color:var(--text-primary);}
.rename-dialog-btns .btn-confirm{background:var(--primary);color:#fff;}

/* Toast */
.toast-container{
position:fixed;top:20px;top:max(20px,env(safe-area-inset-top));left:50%;transform:translateX(-50%);
z-index:999;display:flex;flex-direction:column;gap:8px;align-items:center;pointer-events:none;
}
.toast{
background:linear-gradient(135deg,var(--primary),var(--primary-dark));color:#fff;padding:10px 20px;border-radius:20px;
font-size:13px;font-weight:600;box-shadow:0 6px 18px rgba(0,0,0,0.16);
animation:toastIn 0.3s ease,toastOut 0.3s ease 2.5s forwards;
pointer-events:auto;max-width:90vw;text-align:center;
}
@keyframes toastIn{from{opacity:0;transform:translateY(-10px);}to{opacity:1;transform:translateY(0);}}
@keyframes toastOut{from{opacity:1;transform:translateY(0);}to{opacity:0;transform:translateY(-10px);}}

/* Loading */
.loading-spinner{
width:24px;height:24px;border:3px solid var(--divider);border-top-color:var(--primary);
border-radius:50%;animation:spin 0.8s linear infinite;margin:20px auto;
}
@keyframes spin{to{transform:rotate(360deg);}}

/* Empty state */
.empty-state{text-align:center;padding:30px;color:var(--text-tertiary);font-size:14px;}

/* Long press indicator */
.long-press-hint{font-size:11px;color:var(--text-tertiary);text-align:center;margin-top:4px;}

/* Context menu */
.context-menu{
position:fixed;background:var(--card);border-radius:12px;box-shadow:0 8px 32px rgba(0,0,0,0.15);
padding:6px;z-index:350;display:none;min-width:140px;
}
.context-menu.active{display:block;}
.context-menu-item{
padding:10px 14px;border-radius:8px;font-size:14px;cursor:pointer;color:var(--text-primary);
transition:background 0.15s;
}
.context-menu-item:active{background:var(--card-secondary);}
.context-menu-item.danger{color:var(--primary-dark);}

/* Responsive */
@media(min-width:768px){
.tab-content{padding:0 40px;padding-bottom:90px;}
.tab-nav{padding:0 40px;}
.top-bar{padding:12px 40px;}
.pl-card{width:170px;}.pl-card-cover{height:140px;}
.song-card{width:170px;}.song-card-cover{height:140px;}
}
</style>
</head>
<body>

<div class="app">
  <!-- Top Bar -->
  <div class="top-bar">
    <div class="top-bar-title">ESP32Streaming</div>
  </div>

  <!-- Tab Nav -->
  <div class="tab-nav">
    <button class="tab-btn active" data-tab="home" data-i18n="tabHome">主页</button>
    <button class="tab-btn" data-tab="search" data-i18n="tabSearch">搜索</button>
    <button class="tab-btn" data-tab="settings" data-i18n="tabSettings">设置</button>
  </div>

  <!-- Tab Content -->
  <div class="tab-content" id="tabContent">
    <!-- Home Tab -->
    <div class="tab-panel active" id="tab-home">
      <div class="hero-section">
        <div class="hero-greeting" id="heroGreeting">晚上好</div>
        <div class="hero-subtitle" data-i18n="heroSubtitle">发现并享受你的音乐收藏</div>
      </div>

      <div class="section" id="sectionRecent">
        <div class="section-header">
          <div class="section-title" data-i18n="recentlyPlayed">最近播放</div>
          <button class="section-clear-btn" id="btnClearRecent" style="background:var(--primary);color:#fff;border:none;padding:4px 12px;border-radius:12px;font-size:12px;cursor:pointer;flex-shrink:0;" data-i18n="clearRecent">清空</button>
        </div>
        <div class="hscroll" id="recentCards"></div>
      </div>

      <div class="section" id="sectionPlaylists">
        <div class="section-header">
          <div class="section-title" data-i18n="playlists">歌单</div>
        </div>
        <div class="hscroll" id="playlistCards"></div>
        <div class="long-press-hint" data-i18n="longPressHint">长按歌单可重命名</div>
      </div>

      <div class="section" id="sectionAllMusic">
        <div class="section-header">
          <div class="section-title" data-i18n="allMusic">全部音乐</div>
          <span class="section-more" id="musicCount">0 首</span>
        </div>
        <div class="music-list" id="allMusicList"></div>
        <div class="loading-spinner" id="musicLoading" style="display:none"></div>
      </div>
    </div>

    <!-- Search Tab -->
    <div class="tab-panel" id="tab-search">
      <div class="search-container">
        <div class="search-input-wrap">
          <svg class="search-icon" width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><circle cx="11" cy="11" r="8"/><line x1="21" y1="21" x2="16.65" y2="16.65"/></svg>
          <input type="text" class="search-input" id="searchInput" placeholder="搜索歌曲..." autocomplete="off" data-i18n-placeholder="searchPlaceholder">
          <button class="search-clear" id="searchClear">
            <svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="18" y1="6" x2="6" y2="18"/><line x1="6" y1="6" x2="18" y2="18"/></svg>
          </button>
        </div>
      </div>

      <div class="search-history" id="searchHistory">
        <div class="search-history-title" style="display:flex;align-items:center;justify-content:space-between;">
          <span data-i18n="searchHistory">搜索记录</span>
          <button class="search-history-clear" id="searchHistoryClear" style="background:var(--primary);color:#fff;border:none;padding:4px 12px;border-radius:12px;font-size:12px;cursor:pointer;" data-i18n="clearHistory">清除记录</button>
        </div>
        <div id="searchHistoryList"></div>
      </div>

      <div class="search-results" id="searchResults" style="display:none"></div>
      <div class="empty-state" id="searchEmpty" style="display:none" data-i18n="startTyping">输入内容开始搜索</div>
    </div>

    <!-- Settings Tab -->
    <div class="tab-panel" id="tab-settings">
      <!-- Theme -->
      <div class="settings-section">
        <div class="settings-section-title" data-i18n="language">界面语言</div>
        <div class="language-cards">
          <div class="language-card active" data-lang="zh" id="langCardZh">
            <div class="language-card-title">中文</div>
            <div class="language-card-sub" data-i18n="defaultLang">默认</div>
          </div>
          <div class="language-card" data-lang="en" id="langCardEn">
            <div class="language-card-title">English</div>
            <div class="language-card-sub">English UI</div>
          </div>
        </div>
      </div>

      <!-- Theme -->
      <div class="settings-section">
        <div class="settings-section-title" data-i18n="theme">主题设置</div>
        <div class="theme-cards">
          <div class="theme-card active" data-theme="blue" id="themeCardBlue">
            <div class="theme-card-dot blue"></div>
            <div class="theme-card-label" data-i18n="themeBlue">深海呼吸</div>
            <div class="theme-card-check">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"><polyline points="20 6 9 17 4 12"/></svg>
            </div>
          </div>
          <div class="theme-card" data-theme="green" id="themeCardGreen">
            <div class="theme-card-dot green"></div>
            <div class="theme-card-label" data-i18n="themeGreen">青柠之夏</div>
            <div class="theme-card-check">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"><polyline points="20 6 9 17 4 12"/></svg>
            </div>
          </div>
          <div class="theme-card" data-theme="pink" id="themeCardPink">
            <div class="theme-card-dot pink"></div>
            <div class="theme-card-label" data-i18n="themePink">樱花落雨</div>
            <div class="theme-card-check">
              <svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3" stroke-linecap="round" stroke-linejoin="round"><polyline points="20 6 9 17 4 12"/></svg>
            </div>
          </div>
        </div>
      </div>

      <!-- Network -->
      <div class="settings-section">
        <div class="settings-section-title" data-i18n="network">网络设置</div>
        <div class="settings-row">
          <span class="settings-row-label" data-i18n="wifiStatus">当前 WiFi 状态</span>
          <span id="currentWifiName" style="font-size:14px;color:var(--text-primary);font-weight:600;text-align:center;flex:1;">-</span>
        </div>
        <button class="settings-btn" id="btnToggleNetwork" style="width:100%;margin-top:4px;" data-i18n="more">更多</button>
        <div id="networkDetail" style="display:none;margin-top:8px;">
          <button class="settings-btn" id="btnScanWifi" style="width:100%;margin-bottom:8px;" data-i18n="scanWifi">扫描周围 WiFi</button>
          <div class="wifi-list" id="wifiList"></div>
          <input type="text" class="settings-input" id="wifiSSID" placeholder="SSID" autocomplete="off">
          <input type="password" class="settings-input" id="wifiPassword" placeholder="WiFi 密码" autocomplete="off" data-i18n-placeholder="wifiPassword">
          <div style="display:flex;gap:8px;">
            <button class="settings-btn" id="btnSaveWifi" style="flex:1;" data-i18n="saveConnect">保存并连接</button>
            <button class="settings-btn" id="btnClearWifi" data-i18n="clearWifi">清除 WiFi</button>
          </div>
          <button class="settings-btn" id="btnCollapseNetwork" style="width:100%;margin-top:8px;" data-i18n="collapse">折叠</button>
        </div>
      </div>

      <!-- System -->
      <div class="settings-section">
        <div class="settings-section-title" data-i18n="system">系统设置</div>
        <div class="settings-row">
          <div class="settings-row-text">
            <span class="settings-row-label" data-i18n="debugMode">调试模式</span>
            <span class="settings-row-desc" data-i18n="debugDesc">开启后串口会输出调试信息，请前往服务器后台查看</span>
          </div>
          <label class="toggle">
            <input type="checkbox" id="debugToggle">
            <span class="toggle-slider"></span>
          </label>
        </div>
        <div class="settings-row">
          <div class="settings-row-text">
            <span class="settings-row-label" data-i18n="wifiPower">WiFi 发射功率</span>
            <span class="settings-row-desc" data-i18n="wifiPowerDesc">数值越小功耗越低，若播放卡顿请提高</span>
          </div>
          <select class="settings-select" id="wifiPowerSelect">
            <option value="80">19.5 dBm（最大）</option>
            <option value="76">18.5 dBm</option>
            <option value="68">17 dBm</option>
            <option value="60">15 dBm</option>
            <option value="52">13 dBm</option>
            <option value="44" selected>11 dBm（默认）</option>
            <option value="40">10 dBm</option>
            <option value="36">9 dBm</option>
            <option value="32">8 dBm</option>
            <option value="28">7 dBm</option>
            <option value="20">5 dBm</option>
            <option value="8">2 dBm（最小）</option>
          </select>
        </div>
      </div>

      <!-- Server Info -->
      <div class="settings-section">
        <div class="settings-section-title" data-i18n="serverInfo">服务器信息</div>
        <div class="server-info-card">
          <div class="server-info-row">
            <span class="server-info-label" data-i18n="deviceName">设备名称</span>
            <span class="server-info-value">ESP32Streaming</span>
          </div>
          <div class="server-info-row">
            <span class="server-info-label" data-i18n="ipv4Address">IPv4 地址</span>
            <span class="server-info-value" id="serverIP">--</span>
          </div>
          <div class="server-info-row">
            <span class="server-info-label" data-i18n="mdnsDomain">mDNS 域名</span>
            <span class="server-info-value">esp32streaming.local</span>
          </div>
        </div>
      </div>
    </div>
  </div>
</div>

<!-- Mini Player -->
<div class="mini-player" id="miniPlayer" style="display:none;">
  <div class="mini-player-progress">
    <div class="mini-player-progress-fill" id="miniProgressFill"></div>
  </div>
  <div class="mini-player-main" id="miniPlayerMain">
    <button class="mini-player-btn" id="miniPrevBtn" title="上一首">
      <svg width="18" height="18" viewBox="0 0 24 24" fill="currentColor"><polygon points="19,20 9,12 19,4"/><rect x="4" y="4" width="3" height="16" rx="1"/></svg>
    </button>
    <button class="mini-player-btn" id="miniPlayBtn">
      <svg width="22" height="22" viewBox="0 0 24 24" fill="currentColor"><polygon points="5,3 19,12 5,21"/></svg>
    </button>
    <button class="mini-player-btn" id="miniNextBtn" title="下一首">
      <svg width="18" height="18" viewBox="0 0 24 24" fill="currentColor"><polygon points="5,4 15,12 5,20"/><rect x="17" y="4" width="3" height="16" rx="1"/></svg>
    </button>
    <div class="mini-player-info">
      <div class="mini-player-song" id="miniSongName">未播放歌曲</div>
      <div class="mini-player-eq" id="miniEq">
        <div class="mini-eq-bar"></div>
        <div class="mini-eq-bar"></div>
        <div class="mini-eq-bar"></div>
        <div class="mini-eq-bar"></div>
        <div class="mini-eq-bar"></div>
      </div>
    </div>
    <div class="mini-player-disc" id="miniDisc">
      <svg width="28" height="28" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.5"><circle cx="12" cy="12" r="10"/><circle cx="12" cy="12" r="3"/></svg>
    </div>
  </div>
</div>

<!-- Full Player -->
<div class="full-player" id="fullPlayer">
  <button class="full-player-back" id="fullPlayerBack">
    <svg width="24" height="24" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="6 9 12 15 18 9"/></svg>
  </button>
  <div class="full-player-cover-area">
    <div class="full-player-cover" id="fullPlayerCover">
      <div class="cover-placeholder-fp">
        <svg width="64" height="64" viewBox="0 0 24 24" fill="none" stroke="rgba(255,255,255,0.5)" stroke-width="1.5"><path d="M9 18V5l12-2v13"/><circle cx="6" cy="18" r="3"/><circle cx="18" cy="16" r="3"/></svg>
      </div>
      <div class="eq-bars">
        <div class="eq-bar"></div><div class="eq-bar"></div><div class="eq-bar"></div><div class="eq-bar"></div>
      </div>
    </div>
  </div>
  <div class="full-player-info-area">
    <div class="full-player-song-name" id="fullPlayerSongName">未选择歌曲</div>
    <div class="full-player-song-info" id="fullPlayerSongInfo"></div>
  </div>
  <div class="progress-container">
    <span class="progress-time" id="currentTime">0:00</span>
    <div class="progress-bar-wrap" id="progressBarWrap">
      <div class="progress-bar-bg"></div>
      <div class="progress-bar-fill" id="progressBarFill"></div>
      <div class="progress-bar-thumb" id="progressBarThumb"></div>
    </div>
    <span class="progress-time" id="totalTime">0:00</span>
  </div>
  <div class="controls-row">
    <button class="ctrl-btn mode" id="btnPlayMode" title="顺序播放">
      <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="17 1 21 5 17 9"/><path d="M3 11V9a4 4 0 0 1 4-4h14"/><polyline points="7 23 3 19 7 15"/><path d="M21 13v2a4 4 0 0 1-4 4H3"/></svg>
    </button>
    <button class="ctrl-btn" id="btnPrev">
      <svg width="20" height="20" viewBox="0 0 24 24" fill="currentColor"><polygon points="19,20 9,12 19,4"/><rect x="4" y="4" width="3" height="16" rx="1"/></svg>
    </button>
    <button class="ctrl-btn primary" id="btnPlayPause">
      <svg width="24" height="24" viewBox="0 0 24 24" fill="currentColor"><polygon points="5,3 19,12 5,21"/></svg>
    </button>
    <button class="ctrl-btn" id="btnNext">
      <svg width="20" height="20" viewBox="0 0 24 24" fill="currentColor"><polygon points="5,4 15,12 5,20"/><rect x="17" y="4" width="3" height="16" rx="1"/></svg>
    </button>
    <button class="ctrl-btn small" id="btnDownload">
      <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><path d="M21 15v4a2 2 0 0 1-2 2H5a2 2 0 0 1-2-2v-4"/><polyline points="7 10 12 15 17 10"/><line x1="12" y1="15" x2="12" y2="3"/></svg>
    </button>
  </div>
  <div class="volume-row">
    <svg class="volume-icon" width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polygon points="11 5 6 9 2 9 2 15 6 15 11 19 11 5"/><path d="M19.07 4.93a10 10 0 0 1 0 14.14M15.54 8.46a5 5 0 0 1 0 7.07"/></svg>
    <div class="volume-slider-wrap" id="volumeSliderWrap">
      <div class="volume-slider-bg"></div>
      <div class="volume-slider-fill" id="volumeSliderFill" style="width:80%"></div>
    </div>
  </div>
</div>

<!-- Playlist Modal -->
<div class="modal-overlay" id="playlistModal">
  <div class="modal-sheet">
    <div class="modal-sheet-header">
      <div class="modal-sheet-title" data-i18n="addToPlaylist">添加到歌单</div>
      <button class="modal-close-btn" id="modalCloseBtn">
        <svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="18" y1="6" x2="6" y2="18"/><line x1="6" y1="6" x2="18" y2="18"/></svg>
      </button>
    </div>
    <div id="playlistModalList"></div>
  </div>
</div>

<!-- Rename Dialog -->
<div class="rename-overlay" id="renameOverlay">
  <div class="rename-dialog">
    <div class="rename-dialog-title" data-i18n="renamePlaylist">重命名歌单</div>
    <input type="text" id="renameInput" placeholder="歌单名称" autocomplete="off" data-i18n-placeholder="playlistName">
    <div class="rename-dialog-btns">
      <button class="btn-delete-pl" id="renameDelete" data-i18n="deletePlaylist" style="background:var(--primary-dark);color:#fff;padding:10px 20px;border-radius:20px;border:none;cursor:pointer;font-size:14px;font-weight:500;">删除歌单</button>
      <button class="btn-cancel" id="renameCancel" data-i18n="cancel">取消</button>
    </div>
  </div>
</div>

<!-- Toast Container -->
<div class="toast-container" id="toastContainer"></div>

<!-- Hidden Audio -->
<audio id="audioPlayer" preload="auto" style="display:none"></audio>

<script>
// ==================== STATE ====================
const state = {
  currentTab: 'home',
  language: 'zh',
  theme: 'blue',
  songs: [],
  playlists: [],
  recentPlays: [],
  currentSongIndex: -1,
  currentSong: null,
  isPlaying: false,
  playMode: 'normal', // 'normal' | 'shuffle' | 'repeat'
  volume: 0.8,
  searchResults: [],
  searchHistory: [],
  isSearching: false,
  searchTimer: null,
  currentPlaylistIndex: null,
  showPlaylistModal: false,
  playlistModalSongIndex: null,
  wifiNetworks: [],
  debugMode: false,
  wifiPower: 44,
  currentTime: 0,
  duration: 0,
  renamePlaylistIndex: -1,
  longPressTimer: null,
  allSongsPage: 1,
  allSongsTotal: 0,
  isLoadingMore: false,
};

// ==================== DOM REFS ====================
const $ = (sel) => document.querySelector(sel);
const $$ = (sel) => document.querySelectorAll(sel);

// ==================== LANGUAGE ====================
const I18N = {
  zh: {
    tabHome:'主页', tabSearch:'搜索', tabSettings:'设置',
    heroSubtitle:'发现并享受你的音乐收藏',
    playlists:'歌单', recentlyPlayed:'最近播放', allMusic:'全部音乐', longPressHint:'长按歌单可重命名',
    searchPlaceholder:'搜索歌曲...', searchHistory:'搜索记录', startTyping:'输入内容开始搜索', clearHistory:'清除记录',
    language:'界面语言', defaultLang:'默认', theme:'主题设置',
    themeBlue:'深海呼吸', themeGreen:'青柠之夏', themePink:'樱花落雨',
    network:'网络设置', wifiStatus:'当前 WiFi 状态', currentWifi:'当前 WiFi', scanWifi:'扫描周围 WiFi', wifiPassword:'WiFi 密码',
    saveConnect:'保存并连接', clearWifi:'清除 WiFi', more:'更多', collapse:'折叠', clearRecent:'清空',
    system:'系统设置', debugMode:'调试模式', debugDesc:'开启后串口会输出调试信息，请前往服务器后台查看', wifiPower:'WiFi 发射功率', wifiPowerDesc:'数值越小功耗越低，若播放卡顿请提高',
    connectedDevices:'已连接设备', serverInfo:'服务器信息', deviceName:'设备名称', ipv4Address:'IPv4 地址', mdnsDomain:'mDNS 域名', deviceInfo:'本机信息',
    addToPlaylist:'添加到歌单', renamePlaylist:'重命名歌单', playlistName:'歌单名称', cancel:'取消', confirm:'确认', deletePlaylist:'删除歌单', playlistDeleted:'歌单已删除',
    noSongPlaying:'未播放歌曲', noSongSelected:'未选择歌曲', unknown:'未知',
    songs:'首', songUnit:'首', noRecent:'暂无最近播放', noMusic:'没有找到音乐文件',
    createPlaylist:'+ 创建歌单', addNew:'新建歌单', playlistDefault:'歌单',
    enterPlaylistName:'请输入歌单名称：', playlistCreated:'歌单已创建：', noResults:'未找到结果', noHistory:'暂无搜索记录',
    playlistEmpty:'这个歌单还是空的', removedFromPlaylist:'已从歌单移除', addedToPlaylist:'已添加到歌单',
    themeSwitched:'主题已切换为', scanWifiToast:'正在扫描 WiFi...', foundNetworks:'找到网络数量：', scanFailed:'扫描失败或没有找到网络',
    enterSSID:'请输入 SSID', wifiSaved:'WiFi 已保存，正在连接...', wifiSaveFailed:'保存 WiFi 失败',
    clearWifiConfirm:'确定清除 WiFi 设置吗？', wifiCleared:'WiFi 设置已清除', wifiClearFailed:'清除 WiFi 失败',
    debugEnabled:'调试模式已开启', debugDisabled:'调试模式已关闭', wifiPowerSet:'WiFi 功率已设置为',
    playbackError:'播放错误', renameUnavailable:'当前 main.cpp 暂未提供重命名 API，可先删除后重建歌单',
    disconnected:'未连接', normalMode:'顺序播放', shuffleMode:'随机播放', repeatMode:'单曲循环',
    goodNight:'夜深了', goodMorning:'早上好', goodAfternoon:'下午好', goodEvening:'晚上好'
  },
  en: {
    tabHome:'Home', tabSearch:'Search', tabSettings:'Settings',
    heroSubtitle:'Discover and enjoy your music collection',
    playlists:'Playlists', recentlyPlayed:'Recently Played', allMusic:'All Music', longPressHint:'Long press a playlist to rename it',
    searchPlaceholder:'Search songs...', searchHistory:'Search History', startTyping:'Start typing to search', clearHistory:'Clear',
    language:'Interface Language', defaultLang:'Default', theme:'Theme',
    themeBlue:'Deep Ocean', themeGreen:'Lime Summer', themePink:'Sakura Rain',
    network:'Network', wifiStatus:'WiFi Status', currentWifi:'Current WiFi', scanWifi:'Scan WiFi Networks', wifiPassword:'Password',
    saveConnect:'Save & Connect', clearWifi:'Clear WiFi', more:'More', collapse:'Collapse', clearRecent:'Clear',
    system:'System', debugMode:'Debug Mode', debugDesc:'Serial debug messages will be printed in the server backend console after enabling.', wifiPower:'WiFi TX Power', wifiPowerDesc:'Lower value saves power. Increase it if playback stutters.',
    connectedDevices:'Connected Devices', serverInfo:'Server Info', deviceName:'Device Name', ipv4Address:'IPv4 Address', mdnsDomain:'mDNS Domain', deviceInfo:'Device Info',
    addToPlaylist:'Add to Playlist', renamePlaylist:'Rename Playlist', playlistName:'Playlist name', cancel:'Cancel', confirm:'Confirm', deletePlaylist:'Delete Playlist', playlistDeleted:'Playlist deleted',
    noSongPlaying:'No song playing', noSongSelected:'No song selected', unknown:'Unknown',
    songs:'songs', songUnit:'songs', noRecent:'No recent plays', noMusic:'No music files found',
    createPlaylist:'+ Create Playlist', addNew:'Add new', playlistDefault:'Playlist',
    enterPlaylistName:'Enter playlist name:', playlistCreated:'Playlist created: ', noResults:'No results found', noHistory:'No search history yet',
    playlistEmpty:'This playlist is empty', removedFromPlaylist:'Removed from playlist', addedToPlaylist:'Added to playlist',
    themeSwitched:'Theme switched to', scanWifiToast:'Scanning WiFi...', foundNetworks:'Found networks: ', scanFailed:'Scan failed or no networks found',
    enterSSID:'Please enter SSID', wifiSaved:'WiFi saved. Connecting...', wifiSaveFailed:'Failed to save WiFi',
    clearWifiConfirm:'Clear WiFi settings?', wifiCleared:'WiFi settings cleared', wifiClearFailed:'Failed to clear WiFi',
    debugEnabled:'Debug mode enabled', debugDisabled:'Debug mode disabled', wifiPowerSet:'WiFi power set to',
    playbackError:'Playback error', renameUnavailable:'Rename API is not available yet. Please recreate the playlist.',
    disconnected:'Disconnected', normalMode:'Sequential', shuffleMode:'Shuffle', repeatMode:'Repeat One',
    goodNight:'Good Night', goodMorning:'Good Morning', goodAfternoon:'Good Afternoon', goodEvening:'Good Evening'
  }
};

function t(key) {
  return (I18N[state.language] && I18N[state.language][key]) || I18N.zh[key] || key;
}

const dom = {
  tabContent: $('#tabContent'),
  miniPlayer: $('#miniPlayer'),
  miniProgressFill: $('#miniProgressFill'),
  miniPlayBtn: $('#miniPlayBtn'),
  miniPrevBtn: $('#miniPrevBtn'),
  miniNextBtn: $('#miniNextBtn'),
  miniSongName: $('#miniSongName'),
  fullPlayer: $('#fullPlayer'),
  fullPlayerCover: $('#fullPlayerCover'),
  fullPlayerSongName: $('#fullPlayerSongName'),
  fullPlayerSongInfo: $('#fullPlayerSongInfo'),
  progressBarFill: $('#progressBarFill'),
  progressBarThumb: $('#progressBarThumb'),
  progressBarWrap: $('#progressBarWrap'),
  currentTime: $('#currentTime'),
  totalTime: $('#totalTime'),
  btnPlayPause: $('#btnPlayPause'),
  btnPlayMode: $('#btnPlayMode'),
  btnPrev: $('#btnPrev'),
  btnNext: $('#btnNext'),
  btnDownload: $('#btnDownload'),
  volumeSliderFill: $('#volumeSliderFill'),
  volumeSliderWrap: $('#volumeSliderWrap'),
  audioPlayer: $('#audioPlayer'),
  playlistModal: $('#playlistModal'),
  playlistModalList: $('#playlistModalList'),
  searchInput: $('#searchInput'),
  searchResults: $('#searchResults'),
  searchHistory: $('#searchHistory'),
  searchHistoryList: $('#searchHistoryList'),
  searchEmpty: $('#searchEmpty'),
  searchClear: $('#searchClear'),
  playlistCards: $('#playlistCards'),
  recentCards: $('#recentCards'),
  allMusicList: $('#allMusicList'),
  musicCount: $('#musicCount'),
  heroGreeting: $('#heroGreeting'),
  toastContainer: $('#toastContainer'),
  wifiList: $('#wifiList'),
  wifiSSID: $('#wifiSSID'),
  wifiPassword: $('#wifiPassword'),
  debugToggle: $('#debugToggle'),
  wifiPowerSelect: $('#wifiPowerSelect'),
  serverIP: $('#serverIP'),
  renameOverlay: $('#renameOverlay'),
  renameInput: $('#renameInput'),
  musicLoading: $('#musicLoading'),
};

// ==================== HELPERS ====================
function formatSize(bytes) {
  if (!bytes || bytes === 0) return t('unknown');
  if (bytes < 1024) return bytes + ' B';
  if (bytes < 1048576) return (bytes / 1024).toFixed(1) + ' KB';
  return (bytes / 1048576).toFixed(1) + ' MB';
}

function formatTime(sec) {
  if (!sec || isNaN(sec)) return '0:00';
  const m = Math.floor(sec / 60);
  const s = Math.floor(sec % 60);
  return m + ':' + (s < 10 ? '0' : '') + s;
}

function randomColor() {
  const colors = ['#7B9AAE','#8AAD93','#C4A0A0','#A8C4D4','#B3CDB8','#D8BFBF','#5E8698','#6E947A','#A88484'];
  return colors[Math.floor(Math.random() * colors.length)];
}

function getGreeting() {
  const h = new Date().getHours();
  if (h < 6) return t('goodNight');
  if (h < 12) return t('goodMorning');
  if (h < 18) return t('goodAfternoon');
  return t('goodEvening');
}

function applyLanguage(lang) {
  state.language = lang === 'en' ? 'en' : 'zh';
  document.documentElement.lang = state.language === 'en' ? 'en' : 'zh-CN';
  localStorage.setItem('esp32_lang', state.language);
  $$('[data-i18n]').forEach(el => { el.textContent = t(el.dataset.i18n); });
  $$('[data-i18n-placeholder]').forEach(el => { el.placeholder = t(el.dataset.i18nPlaceholder); });
  $$('.language-card').forEach(c => c.classList.toggle('active', c.dataset.lang === state.language));
  dom.heroGreeting.textContent = getGreeting();
  if (!state.currentSong) {
    dom.miniSongName.textContent = t('noSongPlaying');
    dom.fullPlayerSongName.textContent = t('noSongSelected');
  }
  dom.musicCount.textContent = state.allSongsTotal + ' ' + t('songs');
  updatePlayModeBtn();
  renderPlaylistCards();
  renderRecentCards();
  renderSearchHistory();
  if (state.songs.length) renderAllMusic();
}

function escapeHtml(str) {
  const d = document.createElement('div');
  d.textContent = str;
  return d.innerHTML;
}

// ==================== TOAST ====================
function showToast(msg) {
  const t = document.createElement('div');
  t.className = 'toast';
  t.textContent = msg;
  dom.toastContainer.appendChild(t);
  setTimeout(() => { if (t.parentNode) t.remove(); }, 3000);
}

// ==================== API ====================
async function apiGet(path) {
  try {
    const r = await fetch(path);
    return await r.json();
  } catch (e) {
    console.error('API GET error:', path, e);
    return null;
  }
}

async function apiPost(path, body) {
  try {
    const opts = { method: 'POST' };
    if (body instanceof FormData) {
      opts.body = body;
    } else if (body) {
      opts.headers = { 'Content-Type': 'application/x-www-form-urlencoded' };
      opts.body = new URLSearchParams(body).toString();
    }
    const r = await fetch(path, opts);
    return await r.json();
  } catch (e) {
    console.error('API POST error:', path, e);
    return null;
  }
}

// ==================== MUSIC DATA ====================
async function loadSongs(page = 1) {
  if (page === 1) {
    dom.allMusicList.innerHTML = '<div class="loading-spinner"></div>';
  }
  const data = await apiGet('/api/music?page=' + page + '&size=50');
  if (data && data.files) {
    if (page === 1) {
      state.songs = data.files;
    } else {
      state.songs = state.songs.concat(data.files);
    }
    state.allSongsTotal = data.total || data.files.length;
    dom.musicCount.textContent = state.allSongsTotal + ' ' + t('songs');
    renderAllMusic();
  }
  state.allSongsPage = page;
}

async function loadPlaylists() {
  const data = await apiGet('/api/playlists');
  if (data && data.playlists) {
    state.playlists = data.playlists;
    renderPlaylistCards();
  }
}

async function loadRecent() {
  const data = await apiGet('/api/recent');
  if (data && data.recent) {
    state.recentPlays = data.recent;
    renderRecentCards();
  }
}

async function loadTheme() {
  const data = await apiGet('/api/theme');
  if (data && data.theme) {
    setTheme(data.theme);
  }
}

async function loadSettings() {
  const data = await apiGet('/api/settings');
  console.log('Settings loaded:', data);
  if (data) {
    if (data.debug !== undefined) {
      state.debugMode = data.debug === 1 || data.debug === true;
      dom.debugToggle.checked = state.debugMode;
    }
    if (data.wifi_power !== undefined) {
      state.wifiPower = data.wifi_power;
      dom.wifiPowerSelect.value = String(state.wifiPower);
    }
    // IP显示：优先使用返回的ip，如果没有则尝试从window.location获取
    let ip = data.ip || '';
    if (!ip && window.location.hostname && window.location.hostname !== 'localhost') {
      ip = window.location.hostname;
    }
    if (ip && dom.serverIP) {
      dom.serverIP.textContent = ip;
    } else if (dom.serverIP) {
      dom.serverIP.textContent = '--';
    }
  }
  // 显示当前WiFi名称
  if (data && data.wifi_connected && data.wifi_ssid && data.wifi_ssid.length > 0) {
    document.getElementById('currentWifiName').textContent = data.wifi_ssid;
  } else {
    document.getElementById('currentWifiName').textContent = '-';
  }
}

async function loadAll() {
  await Promise.all([loadSongs(), loadPlaylists(), loadRecent(), loadTheme(), loadSettings()]);
}

// ==================== THEME ====================
function setTheme(theme) {
  state.theme = theme;
  document.documentElement.className = theme === 'green' ? 'theme-green' : theme === 'pink' ? 'theme-pink' : '';
  $$('.theme-card').forEach(c => c.classList.toggle('active', c.dataset.theme === theme));
}

async function switchTheme(theme) {
  setTheme(theme);
  await apiPost('/api/settings/theme?theme=' + theme);
  showToast(t('themeSwitched') + ' ' + (theme === 'green' ? t('themeGreen') : theme === 'pink' ? t('themePink') : t('themeBlue')));
}

// ==================== PLAYER ====================
function playSong(index) {
  if (index < 0 || index >= state.songs.length) return;
  state.currentSongIndex = index;
  state.currentSong = state.songs[index];
  state.isPlaying = false;

  dom.audioPlayer.src = '/api/stream?index=' + index;
  dom.audioPlayer.load();

  const onReady = () => {
    dom.audioPlayer.removeEventListener('canplaythrough', onReady);
    dom.audioPlayer.play().catch(e => console.error('Play error:', e));
    state.isPlaying = true;
    dom.miniPlayer.style.display = 'block';
    dom.miniSongName.textContent = state.currentSong.name || t('unknown');
    dom.fullPlayerSongName.textContent = state.currentSong.name || t('unknown');
    const infoParts = [];
    if (state.currentSong.path) infoParts.push(state.currentSong.path);
    if (state.currentSong.size) infoParts.push(formatSize(state.currentSong.size));
    dom.fullPlayerSongInfo.textContent = infoParts.join(' | ') || '';
    updatePlayPauseIcon();
    updateMiniPlayBtn();
    scrollSongName();
    highlightPlaying();
    updateFullPlayerCover();
  };

  dom.audioPlayer.addEventListener('canplaythrough', onReady);

  // Update UI immediately
  dom.miniPlayer.style.display = 'block';
  dom.miniSongName.textContent = state.currentSong.name || t('unknown');
  dom.fullPlayerSongName.textContent = state.currentSong.name || t('unknown');
  dom.fullPlayerSongInfo.textContent = (state.currentSong.path || '') + ' | ' + (formatSize(state.currentSong.size) || '');
  addRecentInstant(index);
  apiPost('/api/recent/add?index=' + index).then(loadRecent);

  // Clear old pending state
  clearTimeout(state._songLoadTimeout);
  state._songLoadTimeout = setTimeout(() => {
    // Force play after 5 seconds if not ready yet
    dom.audioPlayer.removeEventListener('canplaythrough', onReady);
    dom.audioPlayer.play().catch(e => console.error('Play error:', e));
    state.isPlaying = true;
    updatePlayPauseIcon();
    updateMiniPlayBtn();
  }, 5000);
}

function updateFullPlayerCover() {
  dom.fullPlayerCover.classList.toggle('playing', state.isPlaying);
}

function togglePlay() {
  if (!state.currentSong) {
    if (state.songs.length > 0) playSong(0);
    return;
  }
  if (state.isPlaying) {
    dom.audioPlayer.pause();
    state.isPlaying = false;
  } else {
    dom.audioPlayer.play().catch(e => console.error('Play error:', e));
    state.isPlaying = true;
  }
  updatePlayPauseIcon();
  updateMiniPlayBtn();
  updateFullPlayerCover();
  highlightPlaying();
}

function playNext() {
  if (state.songs.length === 0) return;
  let idx;
  if (state.playMode === 'shuffle') {
    idx = Math.floor(Math.random() * state.songs.length);
  } else {
    idx = state.currentSongIndex + 1;
    if (idx >= state.songs.length) idx = 0;
  }
  if (idx >= 0) playSong(idx);
}

function playPrev() {
  if (state.songs.length === 0) return;
  let idx = state.currentSongIndex - 1;
  if (idx < 0) idx = state.songs.length - 1;
  playSong(idx);
}

function cyclePlayMode() {
  const modes = ['normal', 'shuffle', 'repeat'];
  const idx = modes.indexOf(state.playMode);
  state.playMode = modes[(idx + 1) % 3];
  updatePlayModeBtn();
}

function updatePlayModeBtn() {
  const btn = dom.btnPlayMode;
  if (state.playMode === 'shuffle') {
    btn.innerHTML = '<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="16 3 21 3 21 8"/><line x1="4" y1="20" x2="21" y2="3"/><polyline points="21 16 21 21 16 21"/><line x1="15" y1="15" x2="21" y2="21"/><line x1="4" y1="4" x2="9" y2="9"/></svg>';
    btn.title = t('shuffleMode');
  } else if (state.playMode === 'repeat') {
    btn.innerHTML = '<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="17 1 21 5 17 9"/><path d="M3 11V9a4 4 0 0 1 4-4h14"/><polyline points="7 23 3 19 7 15"/><path d="M21 13v2a4 4 0 0 1-4 4H3"/><circle cx="9" cy="12" r="1.5" fill="currentColor" stroke="none"/></svg>';
    btn.title = t('repeatMode');
  } else {
    btn.innerHTML = '<svg width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round" stroke-linejoin="round"><polyline points="17 1 21 5 17 9"/><path d="M3 11V9a4 4 0 0 1 4-4h14"/><polyline points="7 23 3 19 7 15"/><path d="M21 13v2a4 4 0 0 1-4 4H3"/></svg>';
    btn.title = t('normalMode');
  }
}

function updatePlayPauseIcon() {
  if (state.isPlaying) {
    dom.btnPlayPause.innerHTML = '<svg width="24" height="24" viewBox="0 0 24 24" fill="currentColor"><rect x="6" y="4" width="4" height="16" rx="1"/><rect x="14" y="4" width="4" height="16" rx="1"/></svg>';
  } else {
    dom.btnPlayPause.innerHTML = '<svg width="24" height="24" viewBox="0 0 24 24" fill="currentColor"><polygon points="5,3 19,12 5,21"/></svg>';
  }
}

function updateMiniPlayBtn() {
  if (state.isPlaying) {
    dom.miniPlayBtn.innerHTML = '<svg width="22" height="22" viewBox="0 0 24 24" fill="currentColor"><rect x="6" y="4" width="4" height="16" rx="1"/><rect x="14" y="4" width="4" height="16" rx="1"/></svg>';
    document.getElementById('miniEq').classList.add('active');
    document.getElementById('miniDisc').classList.add('active');
  } else {
    dom.miniPlayBtn.innerHTML = '<svg width="22" height="22" viewBox="0 0 24 24" fill="currentColor"><polygon points="5,3 19,12 5,21"/></svg>';
    document.getElementById('miniEq').classList.remove('active');
    document.getElementById('miniDisc').classList.remove('active');
  }
}

function scrollSongName() {
  const el = dom.miniSongName;
  const el2 = dom.fullPlayerSongName;
  el.classList.remove('scroll');
  el2.classList.remove('scroll');
  void el.offsetWidth;
  void el2.offsetWidth;
  if (el.scrollWidth > el.clientWidth) el.classList.add('scroll');
  if (el2.scrollWidth > el2.clientWidth) el2.classList.add('scroll');
}

function highlightPlaying() {
  $$('.music-item').forEach(el => el.classList.remove('playing'));
  $$('.song-card').forEach(el => el.classList.remove('playing'));
  if (state.currentSongIndex >= 0) {
    const items = $$('.music-item');
    items.forEach(el => {
      if (parseInt(el.dataset.index) === state.currentSongIndex) el.classList.add('playing');
    });
    const cards = $$('.song-card');
    cards.forEach(el => {
      if (parseInt(el.dataset.index) === state.currentSongIndex) el.classList.add('playing');
    });
  }
}

function setVolume(val) {
  state.volume = Math.max(0, Math.min(1, val));
  dom.audioPlayer.volume = state.volume;
  dom.volumeSliderFill.style.width = (state.volume * 100) + '%';
}

// ==================== RENDER ====================
function renderPlaylistCards() {
  let html = '';
  state.playlists.forEach((pl, i) => {
    const name = escapeHtml(pl.name || t('playlistDefault') + ' ' + (i + 1));
    const count = pl.count || 0;
    html += '<div class="pl-card" data-pl-index="' + i + '" data-pl-name="' + escapeHtml(pl.name || '') + '">';
    html += '<div class="pl-card-cover">';
    html += '<svg viewBox="0 0 24 24" fill="none" stroke="rgba(255,255,255,0.7)" stroke-width="1.5"><path d="M9 18V5l12-2v13"/><circle cx="6" cy="18" r="3"/><circle cx="18" cy="16" r="3"/></svg>';
    html += '</div>';
    html += '<div class="pl-card-info"><div class="pl-card-name">' + name + '</div><div class="pl-card-count">' + count + ' ' + t('songUnit') + '</div></div>';
    html += '</div>';
  });
  html += '<div class="pl-card pl-card-create" id="plCardCreate">';
  html += '<div class="pl-card-cover">';
  html += '<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/></svg>';
  html += '</div>';
  html += '<div class="pl-card-info"><div class="pl-card-name">' + t('createPlaylist') + '</div><div class="pl-card-count">' + t('addNew') + '</div></div>';
  html += '</div>';
  dom.playlistCards.innerHTML = html;

  // Bind playlist card events
  $$('.pl-card:not(.pl-card-create)').forEach(card => {
    card.addEventListener('click', () => {
      const idx = parseInt(card.dataset.plIndex);
      loadPlaylistSongs(idx);
    });
    // Long press for rename
    card.addEventListener('touchstart', (e) => startLongPress(e, card));
    card.addEventListener('touchend', cancelLongPress);
    card.addEventListener('touchmove', cancelLongPress);
    card.addEventListener('mousedown', (e) => startLongPress(e, card));
    card.addEventListener('mouseup', cancelLongPress);
    card.addEventListener('mouseleave', cancelLongPress);
  });

  const createBtn = $('#plCardCreate');
  if (createBtn) {
    createBtn.addEventListener('click', () => {
      const name = prompt(t('enterPlaylistName'));
      if (name && name.trim()) {
        apiPost('/api/playlist/create?name=' + encodeURIComponent(name.trim())).then(() => {
          showToast(t('playlistCreated') + name.trim());
          loadPlaylists();
        });
      }
    });
  }
}

function addRecentInstant(index) {
  if (index < 0 || index >= state.songs.length) return;
  const song = Object.assign({}, state.songs[index], { index });
  state.recentPlays = state.recentPlays.filter(item => item.index !== index);
  state.recentPlays.unshift(song);
  if (state.recentPlays.length > 20) state.recentPlays.pop();
  renderRecentCards();
}

function renderRecentCards() {
  if (state.recentPlays.length === 0) {
    dom.recentCards.innerHTML = '<div class="empty-state" style="padding:10px">' + t('noRecent') + '</div>';
    return;
  }
  let html = '';
  state.recentPlays.forEach((song, i) => {
    const name = escapeHtml(song.name || t('unknown'));
    const meta = formatSize(song.size);
    const color = randomColor();
    html += '<div class="song-card" data-index="' + (song.index !== undefined ? song.index : i) + '">';
    html += '<div class="song-card-cover" style="background:linear-gradient(135deg,' + color + ',' + color + '88)">';
    html += '<div class="cover-placeholder"><svg width="50" height="50" viewBox="0 0 24 24" fill="none" stroke="rgba(255,255,255,0.5)" stroke-width="1.5"><path d="M9 18V5l12-2v13"/><circle cx="6" cy="18" r="3"/><circle cx="18" cy="16" r="3"/></svg></div>';
    html += '<div class="eq-bars"><div class="eq-bar"></div><div class="eq-bar"></div><div class="eq-bar"></div><div class="eq-bar"></div></div>';
    html += '</div>';
    html += '<div class="song-card-info"><div class="song-card-name">' + name + '</div><div class="song-card-artist">' + meta + '</div></div>';
    html += '</div>';
  });
  dom.recentCards.innerHTML = html;

  $$('#recentCards .song-card').forEach(card => {
    card.addEventListener('click', () => {
      const idx = parseInt(card.dataset.index);
      if (idx >= 0 && idx < state.songs.length) playSong(idx);
    });
  });
  highlightPlaying();
}

function renderAllMusic() {
  if (state.songs.length === 0) {
    dom.allMusicList.innerHTML = '<div class="empty-state">' + t('noMusic') + '</div>';
    return;
  }
  let html = '';
  state.songs.forEach((song, i) => {
    const name = escapeHtml(song.name || t('unknown'));
    const size = formatSize(song.size);
    const color = randomColor();
    html += '<div class="music-item" data-index="' + i + '">';
    html += '<div class="music-cover" style="background:linear-gradient(135deg,' + color + ',' + color + '88)">';
    html += '<div class="cover-placeholder-sm"><svg width="28" height="28" viewBox="0 0 24 24" fill="none" stroke="rgba(255,255,255,0.5)" stroke-width="1.5"><path d="M9 18V5l12-2v13"/><circle cx="6" cy="18" r="3"/><circle cx="18" cy="16" r="3"/></svg></div>';
    html += '<div class="eq-bars"><div class="eq-bar"></div><div class="eq-bar"></div><div class="eq-bar"></div><div class="eq-bar"></div></div>';
    html += '</div>';
    html += '<div class="music-info"><div class="music-name">' + name + '</div><div class="music-meta">' + size + '</div></div>';
    html += '<button class="music-add-btn" data-index="' + i + '">';
    html += '<svg width="16" height="16" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.5"><line x1="12" y1="5" x2="12" y2="19"/><line x1="5" y1="12" x2="19" y2="12"/></svg>';
    html += '</button>';
    html += '</div>';
  });
  dom.allMusicList.innerHTML = html;

  // Bind click on song
  $$('#allMusicList .music-item').forEach(item => {
    item.addEventListener('click', (e) => {
      if (e.target.closest('.music-add-btn')) return;
      const idx = parseInt(item.dataset.index);
      playSong(idx);
    });
  });

  // Bind add button
  $$('#allMusicList .music-add-btn').forEach(btn => {
    btn.addEventListener('click', (e) => {
      e.stopPropagation();
      const idx = parseInt(btn.dataset.index);
      state.playlistModalSongIndex = idx;
      openPlaylistModal();
    });
  });

  highlightPlaying();
}

function renderPlaylistSongs(playlistIndex, songs) {
  const pl = state.playlists[playlistIndex];
  if (!pl) return;
  // Navigate to home and show filtered view
  switchTab('home');
  let html = '<div style="display:flex;align-items:center;gap:8px;margin-bottom:12px">';
  html += '<button style="background:none;border:none;cursor:pointer;color:var(--text-secondary);padding:4px" onclick="loadAll()">';
  html += '<svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><polyline points="15 18 9 12 15 6"/></svg>';
  html += '</button>';
  html += '<span style="font-size:18px;font-weight:700">' + escapeHtml(pl.name) + '</span>';
  html += '</div>';
  html += '<div class="music-list">';
  if (songs && songs.length > 0) {
    songs.forEach((song, i) => {
      const name = escapeHtml(song.name || t('unknown'));
      const size = formatSize(song.size);
      const color = randomColor();
      html += '<div class="music-item" data-index="' + (song.index !== undefined ? song.index : i) + '">';
      html += '<div class="music-cover" style="background:linear-gradient(135deg,' + color + ',' + color + '88)">';
      html += '<div class="cover-placeholder-sm"><svg width="28" height="28" viewBox="0 0 24 24" fill="none" stroke="rgba(255,255,255,0.5)" stroke-width="1.5"><path d="M9 18V5l12-2v13"/><circle cx="6" cy="18" r="3"/><circle cx="18" cy="16" r="3"/></svg></div>';
      html += '<div class="eq-bars"><div class="eq-bar"></div><div class="eq-bar"></div><div class="eq-bar"></div><div class="eq-bar"></div></div>';
      html += '</div>';
      html += '<div class="music-info"><div class="music-name">' + name + '</div><div class="music-meta">' + size + '</div></div>';
      html += '</div>';
    });
  } else {
    html += '<div class="empty-state">' + t('playlistEmpty') + '</div>';
  }
  html += '</div>';
  dom.allMusicList.innerHTML = html;
  state.currentPlaylistIndex = playlistIndex;

  $$('#allMusicList .music-item').forEach(item => {
    item.addEventListener('click', () => {
      const idx = parseInt(item.dataset.index);
      playSong(idx);
    });
  });
  highlightPlaying();
}

async function loadPlaylistSongs(playlistIndex) {
  const data = await apiGet('/api/playlist/songs?index=' + playlistIndex);
  if (data) {
    renderPlaylistSongs(playlistIndex, data.songs || data.files || []);
  }
}

function renderPlaylistModal() {
  let html = '';
  state.playlists.forEach((pl, i) => {
    const name = escapeHtml(pl.name || t('playlistDefault') + ' ' + (i + 1));
    html += '<div class="modal-playlist-item">';
    html += '<div class="modal-playlist-name">' + name + '</div>';
    html += '<button class="modal-playlist-toggle" data-pl-index="' + i + '">';
    html += '<svg width="14" height="14" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="3"><polyline points="20 6 9 17 4 12"/></svg>';
    html += '</button>';
    html += '</div>';
  });
  dom.playlistModalList.innerHTML = html;
}

function openPlaylistModal() {
  renderPlaylistModal();
  dom.playlistModal.classList.add('active');
  // Check which playlists contain the song
  setTimeout(() => {
    $$('.modal-playlist-toggle').forEach(btn => {
      const plIdx = parseInt(btn.dataset.plIndex);
      // We'll toggle optimistically; actual state would need a different API
      btn.classList.remove('active');
    });
  }, 100);
}

function closePlaylistModal() {
  dom.playlistModal.classList.remove('active');
  state.playlistModalSongIndex = null;
}

// ==================== SEARCH ====================
async function doSearch(q) {
  if (!q || q.trim().length === 0) {
    dom.searchResults.style.display = 'none';
    dom.searchHistory.style.display = 'block';
    dom.searchEmpty.style.display = 'none';
    state.isSearching = false;
    return;
  }
  state.isSearching = true;
  dom.searchHistory.style.display = 'none';
  dom.searchEmpty.style.display = 'none';
  const data = await apiGet('/api/search?q=' + encodeURIComponent(q.trim()));
  if (data && data.results) {
    state.searchResults = data.results;
    renderSearchResults();
  } else if (data && data.files) {
    state.searchResults = data.files;
    renderSearchResults();
  }
}

function renderSearchResults() {
  if (state.searchResults.length === 0) {
    dom.searchResults.style.display = 'none';
    dom.searchEmpty.style.display = 'block';
    dom.searchEmpty.textContent = t('noResults');
    return;
  }
  dom.searchResults.style.display = 'block';
  dom.searchEmpty.style.display = 'none';
  let html = '';
  state.searchResults.forEach((song, i) => {
    const name = escapeHtml(song.name || t('unknown'));
    const size = formatSize(song.size);
    const color = randomColor();
    html += '<div class="search-result-item" data-index="' + (song.index !== undefined ? song.index : i) + '">';
    html += '<div class="search-result-cover" style="background:linear-gradient(135deg,' + color + ',' + color + '88)"></div>';
    html += '<div class="search-result-info"><div class="search-result-name">' + name + '</div><div class="search-result-meta">' + size + '</div></div>';
    html += '</div>';
  });
  dom.searchResults.innerHTML = html;

  $$('#searchResults .search-result-item').forEach(item => {
    item.addEventListener('click', () => {
      const idx = parseInt(item.dataset.index);
      playSong(idx);
      addSearchHistory(state.songs[idx] || state.searchResults[idx]);
    });
  });
}

function addSearchHistory(song) {
  if (!song) return;
  const entry = { name: song.name || t('unknown'), index: song.index };
  // Remove if already exists
  state.searchHistory = state.searchHistory.filter(h => h.index !== entry.index);
  state.searchHistory.unshift(entry);
  if (state.searchHistory.length > 20) state.searchHistory.pop();
  renderSearchHistory();
}

function renderSearchHistory() {
  if (state.searchHistory.length === 0) {
    dom.searchHistoryList.innerHTML = '<div class="search-history-empty">' + t('noHistory') + '</div>';
    return;
  }
  let html = '';
  state.searchHistory.forEach(h => {
    html += '<div class="search-history-item" data-index="' + h.index + '">' + escapeHtml(h.name) + '</div>';
  });
  dom.searchHistoryList.innerHTML = html;

  $$('#searchHistoryList .search-history-item').forEach(item => {
    item.addEventListener('click', () => {
      const idx = parseInt(item.dataset.index);
      if (idx >= 0 && idx < state.songs.length) playSong(idx);
    });
  });
}

// ==================== LONG PRESS ====================
function startLongPress(e, card) {
  state.longPressTimer = setTimeout(() => {
    const idx = parseInt(card.dataset.plIndex);
    const name = card.dataset.plName || '';
    state.renamePlaylistIndex = idx;
    dom.renameInput.value = name;
    dom.renameOverlay.classList.add('active');
  }, 800);
}

function cancelLongPress() {
  if (state.longPressTimer) {
    clearTimeout(state.longPressTimer);
    state.longPressTimer = null;
  }
}

// ==================== TAB SWITCHING ====================
function switchTab(tab) {
  state.currentTab = tab;
  $$('.tab-btn').forEach(b => b.classList.toggle('active', b.dataset.tab === tab));
  $$('.tab-panel').forEach(p => p.classList.toggle('active', p.id === 'tab-' + tab));

  // Show/hide mini player
  dom.miniPlayer.style.display = (tab === 'settings') ? 'none' : (state.currentSong ? 'block' : 'none');

  if (tab === 'home') {
    loadPlaylists();
    loadRecent();
  }
  if (tab === 'search') {
    renderSearchHistory();
    dom.searchInput.focus();
  }
}

// ==================== EVENT BINDINGS ====================
function bindEvents() {
  // Tabs
  $$('.tab-btn').forEach(btn => {
    btn.addEventListener('click', () => switchTab(btn.dataset.tab));
  });

  // 清空最近播放
  $('#btnClearRecent').addEventListener('click', async () => {
    await apiPost('/api/recent/clear');
    state.recentPlays = [];
    renderRecentCards();
    showToast(t('clearRecent'));
  });

  // Mini player
  dom.miniPlayBtn.addEventListener('click', (e) => {
    e.stopPropagation();
    togglePlay();
  });
  dom.miniPrevBtn.addEventListener('click', (e) => { e.stopPropagation(); playPrev(); });
  dom.miniNextBtn.addEventListener('click', (e) => { e.stopPropagation(); playNext(); });
  $('#miniPlayerMain').addEventListener('click', () => {
    dom.fullPlayer.classList.add('active');
  });

  // Full player
  $('#fullPlayerBack').addEventListener('click', () => {
    dom.fullPlayer.classList.remove('active');
  });
  dom.btnPlayPause.addEventListener('click', togglePlay);
  dom.btnPrev.addEventListener('click', playPrev);
  dom.btnNext.addEventListener('click', playNext);
  dom.btnPlayMode.addEventListener('click', cyclePlayMode);
  dom.btnDownload.addEventListener('click', () => {
    if (state.currentSong) {
      window.open('/api/download?index=' + state.currentSongIndex, '_blank');
    }
  });

  // Progress bar
  dom.progressBarWrap.addEventListener('click', (e) => {
    const rect = dom.progressBarWrap.getBoundingClientRect();
    const ratio = (e.clientX - rect.left) / rect.width;
    seekTo(ratio);
  });
  let progressDragging = false;
  dom.progressBarWrap.addEventListener('touchstart', (e) => {
    progressDragging = true;
    updateProgressFromTouch(e);
  });
  dom.progressBarWrap.addEventListener('touchmove', (e) => {
    if (progressDragging) updateProgressFromTouch(e);
  });
  dom.progressBarWrap.addEventListener('touchend', (e) => {
    if (progressDragging) {
      progressDragging = false;
      const rect = dom.progressBarWrap.getBoundingClientRect();
      const ratio = Math.max(0, Math.min(1, (e.changedTouches[0].clientX - rect.left) / rect.width));
      seekTo(ratio);
    }
  });
  dom.progressBarWrap.addEventListener('mousedown', (e) => {
    progressDragging = true;
    updateProgressFromMouse(e);
  });
  document.addEventListener('mousemove', (e) => {
    if (progressDragging) updateProgressFromMouse(e);
  });
  document.addEventListener('mouseup', (e) => {
    if (progressDragging) {
      progressDragging = false;
      const rect = dom.progressBarWrap.getBoundingClientRect();
      const ratio = Math.max(0, Math.min(1, (e.clientX - rect.left) / rect.width));
      seekTo(ratio);
    }
  });

  function updateProgressFromTouch(e) {
    const rect = dom.progressBarWrap.getBoundingClientRect();
    const ratio = Math.max(0, Math.min(1, (e.touches[0].clientX - rect.left) / rect.width));
    dom.progressBarFill.style.width = (ratio * 100) + '%';
    dom.progressBarThumb.style.left = (ratio * 100) + '%';
  }

  function updateProgressFromMouse(e) {
    const rect = dom.progressBarWrap.getBoundingClientRect();
    const ratio = Math.max(0, Math.min(1, (e.clientX - rect.left) / rect.width));
    dom.progressBarFill.style.width = (ratio * 100) + '%';
    dom.progressBarThumb.style.left = (ratio * 100) + '%';
  }

  function seekTo(ratio) {
    if (dom.audioPlayer.duration) {
      dom.audioPlayer.currentTime = ratio * dom.audioPlayer.duration;
    }
  }

  // Volume slider
  dom.volumeSliderWrap.addEventListener('click', (e) => {
    const rect = dom.volumeSliderWrap.getBoundingClientRect();
    setVolume((e.clientX - rect.left) / rect.width);
  });
  let volDragging = false;
  dom.volumeSliderWrap.addEventListener('touchstart', (e) => {
    volDragging = true;
    const rect = dom.volumeSliderWrap.getBoundingClientRect();
    setVolume((e.touches[0].clientX - rect.left) / rect.width);
  });
  dom.volumeSliderWrap.addEventListener('touchmove', (e) => {
    if (volDragging) {
      const rect = dom.volumeSliderWrap.getBoundingClientRect();
      setVolume((e.touches[0].clientX - rect.left) / rect.width);
    }
  });
  document.addEventListener('touchend', (e) => {
    if (volDragging) {
      volDragging = false;
      const rect = dom.volumeSliderWrap.getBoundingClientRect();
      const ratio = Math.max(0, Math.min(1, (e.changedTouches[0].clientX - rect.left) / rect.width));
      setVolume(ratio);
    }
  });

  // Audio events
  dom.audioPlayer.addEventListener('timeupdate', () => {
    state.currentTime = dom.audioPlayer.currentTime;
    state.duration = dom.audioPlayer.duration;
    if (state.duration) {
      const pct = (state.currentTime / state.duration) * 100;
      dom.progressBarFill.style.width = pct + '%';
      dom.progressBarThumb.style.left = pct + '%';
      dom.miniProgressFill.style.width = pct + '%';
    }
    dom.currentTime.textContent = formatTime(state.currentTime);
    dom.totalTime.textContent = formatTime(state.duration);
  });

  dom.audioPlayer.addEventListener('ended', () => {
    if (state.playMode === 'repeat') {
      dom.audioPlayer.currentTime = 0;
      dom.audioPlayer.play().catch(() => {});
    } else {
      playNext();
    }
  });

  dom.audioPlayer.addEventListener('play', () => {
    state.isPlaying = true;
    updatePlayPauseIcon();
    updateMiniPlayBtn();
    updateFullPlayerCover();
    highlightPlaying();
  });

  dom.audioPlayer.addEventListener('pause', () => {
    state.isPlaying = false;
    updatePlayPauseIcon();
    updateMiniPlayBtn();
    updateFullPlayerCover();
    highlightPlaying();
  });

  dom.audioPlayer.addEventListener('error', () => {
    showToast(t('playbackError'));
    state.isPlaying = false;
    updatePlayPauseIcon();
    updateMiniPlayBtn();
    updateFullPlayerCover();
  });

  // Search
  dom.searchInput.addEventListener('input', () => {
    clearTimeout(state.searchTimer);
    const q = dom.searchInput.value;
    if (q.trim().length === 0) {
      dom.searchResults.style.display = 'none';
      dom.searchHistory.style.display = 'block';
      dom.searchEmpty.style.display = 'none';
      state.isSearching = false;
      return;
    }
    state.searchTimer = setTimeout(() => doSearch(q), 300);
  });
  dom.searchInput.addEventListener('focus', () => {
    if (dom.searchInput.value.trim().length === 0) {
      dom.searchHistory.style.display = 'block';
      dom.searchResults.style.display = 'none';
      dom.searchEmpty.style.display = 'none';
    }
  });
  dom.searchClear.addEventListener('click', () => {
    dom.searchInput.value = '';
    dom.searchResults.style.display = 'none';
    dom.searchHistory.style.display = 'block';
    dom.searchEmpty.style.display = 'none';
    state.isSearching = false;
    dom.searchInput.focus();
  });

  // Search history clear
  $('#searchHistoryClear').addEventListener('click', () => {
    state.searchHistory = [];
    renderSearchHistory();
    localStorage.removeItem('esp32_search_history');
  });

  // Playlist modal
  $('#modalCloseBtn').addEventListener('click', closePlaylistModal);
  dom.playlistModal.addEventListener('click', (e) => {
    if (e.target === dom.playlistModal) closePlaylistModal();
  });
  dom.playlistModalList.addEventListener('click', (e) => {
    const btn = e.target.closest('.modal-playlist-toggle');
    if (!btn) return;
    const plIdx = parseInt(btn.dataset.plIndex);
    const songIdx = state.playlistModalSongIndex;
    if (songIdx === null || songIdx === undefined) return;
    const isActive = btn.classList.contains('active');
    if (isActive) {
      apiPost('/api/playlist/remove?pl=' + plIdx + '&song=' + songIdx).then(() => {
        btn.classList.remove('active');
        showToast(t('removedFromPlaylist'));
        loadPlaylists();
      });
    } else {
      apiPost('/api/playlist/add?pl=' + plIdx + '&song=' + songIdx).then(() => {
        btn.classList.add('active');
        showToast(t('addedToPlaylist'));
        loadPlaylists();
      });
    }
  });

  // Theme cards
  $$('.theme-card').forEach(card => {
    card.addEventListener('click', () => switchTheme(card.dataset.theme));
  });

  // Language cards
  $$('.language-card').forEach(card => {
    card.addEventListener('click', () => applyLanguage(card.dataset.lang));
  });

  // Rename dialog
  $('#renameCancel').addEventListener('click', () => {
    dom.renameOverlay.classList.remove('active');
    state.renamePlaylistIndex = -1;
  });
  $('#renameDelete').addEventListener('click', async () => {
    if (state.renamePlaylistIndex >= 0) {
      await apiPost('/api/playlist/delete?index=' + state.renamePlaylistIndex);
      loadPlaylists();
      dom.renameOverlay.classList.remove('active');
      state.renamePlaylistIndex = -1;
      showToast(t('playlistDeleted'));
    }
  });
  dom.renameOverlay.addEventListener('click', (e) => {
    if (e.target === dom.renameOverlay) {
      dom.renameOverlay.classList.remove('active');
      state.renamePlaylistIndex = -1;
    }
  });

  // 网络设置折叠
  $('#btnToggleNetwork').addEventListener('click', () => {
    const detail = document.getElementById('networkDetail');
    const btn = document.getElementById('btnToggleNetwork');
    detail.style.display = 'block';
    btn.style.display = 'none';
  });
  $('#btnCollapseNetwork').addEventListener('click', () => {
    document.getElementById('networkDetail').style.display = 'none';
    const btn = document.getElementById('btnToggleNetwork');
    btn.style.display = 'block';
    btn.textContent = t('more');
  });

  // WiFi
  $('#btnScanWifi').addEventListener('click', async () => {
    showToast(t('scanWifiToast'));
    const data = await apiGet('/api/scanwifi');
    if (data && data.networks) {
      state.wifiNetworks = data.networks;
      renderWifiList();
      showToast(t('foundNetworks') + data.networks.length);
    } else {
      showToast(t('scanFailed'));
    }
  });

  $('#btnSaveWifi').addEventListener('click', async () => {
    const ssid = dom.wifiSSID.value.trim();
    const pwd = dom.wifiPassword.value.trim();
    if (!ssid) { showToast(t('enterSSID')); return; }
    const formData = new FormData();
    formData.append('ssid', ssid);
    formData.append('password', pwd);
    const data = await apiPost('/api/savewifi', formData);
    if (data && data.status === 'ok') {
      showToast(t('wifiSaved'));
    } else {
      showToast(t('wifiSaveFailed'));
    }
  });

  $('#btnClearWifi').addEventListener('click', async () => {
    if (!confirm(t('clearWifiConfirm'))) return;
    const data = await apiPost('/api/clearwifi');
    if (data && data.status === 'ok') {
      showToast(t('wifiCleared'));
      document.getElementById('currentWifiName').textContent = '-';
    } else {
      showToast(t('wifiClearFailed'));
    }
  });

  // Debug toggle
  dom.debugToggle.addEventListener('change', async () => {
    state.debugMode = dom.debugToggle.checked;
    await apiPost('/api/settings/debug?enabled=' + (state.debugMode ? '1' : '0'));
    showToast(state.debugMode ? t('debugEnabled') : t('debugDisabled'));
  });

  // WiFi power
  dom.wifiPowerSelect.addEventListener('change', async () => {
    state.wifiPower = parseInt(dom.wifiPowerSelect.value);
    await apiPost('/api/settings/wifipower?power=' + state.wifiPower);
    showToast(t('wifiPowerSet') + ' ' + dom.wifiPowerSelect.selectedOptions[0].text);
  });

  // Infinite scroll
  dom.tabContent.addEventListener('scroll', () => {
    if (state.currentTab !== 'home') return;
    if (state.isLoadingMore) return;
    const el = dom.tabContent;
    if (el.scrollTop + el.clientHeight >= el.scrollHeight - 100) {
      if (state.songs.length < state.allSongsTotal) {
        state.isLoadingMore = true;
        dom.musicLoading.style.display = 'block';
        loadMoreSongs();
      }
    }
  });

  // Keyboard shortcut
  document.addEventListener('keydown', (e) => {
    if (e.target.tagName === 'INPUT' || e.target.tagName === 'TEXTAREA') return;
    if (e.code === 'Space') { e.preventDefault(); togglePlay(); }
    if (e.code === 'ArrowRight') { e.preventDefault(); playNext(); }
    if (e.code === 'ArrowLeft') { e.preventDefault(); playPrev(); }
  });
}

async function loadMoreSongs() {
  const nextPage = state.allSongsPage + 1;
  await loadSongs(nextPage);
  state.isLoadingMore = false;
  dom.musicLoading.style.display = 'none';
}

function renderWifiList() {
  if (state.wifiNetworks.length === 0) {
    dom.wifiList.innerHTML = '<div class="empty-state" style="padding:10px;font-size:12px">' + (state.language === 'en' ? 'No networks found' : '没有找到 WiFi 网络') + '</div>';
    return;
  }
  let html = '';
  state.wifiNetworks.forEach(net => {
    const ssid = escapeHtml(net.ssid || t('unknown'));
    const rssi = net.rssi || net.signal || -100;
    const strength = state.language === 'en'
      ? (rssi > -50 ? 'Excellent' : rssi > -65 ? 'Good' : rssi > -75 ? 'Fair' : 'Weak')
      : (rssi > -50 ? '极强' : rssi > -65 ? '良好' : rssi > -75 ? '一般' : '较弱');
    const bars = rssi > -50 ? 4 : rssi > -65 ? 3 : rssi > -75 ? 2 : 1;
    html += '<div class="wifi-item" data-ssid="' + escapeHtml(net.ssid || '') + '">';
    html += '<svg class="wifi-signal" width="18" height="18" viewBox="0 0 24 24" fill="currentColor" opacity="0.5">';
    html += '<path d="M12 18c-.55 0-1 .45-1 1s.45 1 1 1 1-.45 1-1-.45-1-1-1z"/>';
    if (bars >= 2) html += '<path d="M8.5 14.5c-.35.35-.35.9 0 1.25.35.35.9.35 1.25 0 .35-.35.35-.9 0-1.25-.35-.35-.9-.35-1.25 0zm7 0c-.35.35-.35.9 0 1.25.35.35.9.35 1.25 0 .35-.35.35-.9 0-1.25-.35-.35-.9-.35-1.25 0z"/>';
    if (bars >= 3) html += '<path d="M5 10.5c-.35.35-.35.9 0 1.25s.9.35 1.25 0L12 6l5.75 5.75c.35.35.9.35 1.25 0s.35-.9 0-1.25L12 3.5 5 10.5z"/>';
    if (bars >= 4) html += '<path d="M1.5 6.5c-.35.35-.35.9 0 1.25L12 18l10.5-10.25c.35-.35.35-.9 0-1.25s-.9-.35-1.25 0L12 15.5 2.75 6.5c-.35-.35-.9-.35-1.25 0z"/>';
    html += '</svg>';
    html += '<span class="wifi-ssid">' + ssid + '</span>';
    html += '<span class="wifi-strength">' + strength + ' (' + rssi + 'dBm)</span>';
    html += '</div>';
  });
  dom.wifiList.innerHTML = html;

  $$('#wifiList .wifi-item').forEach(item => {
    item.addEventListener('click', () => {
      dom.wifiSSID.value = item.dataset.ssid || '';
      dom.wifiPassword.focus();
    });
  });
}

// ==================== INIT ====================
function init() {
  state.language = localStorage.getItem('esp32_lang') || 'zh';
  dom.audioPlayer.volume = state.volume;
  setVolume(state.volume);
  bindEvents();
  applyLanguage(state.language);
  renderSearchHistory();
  loadAll();
  updatePlayModeBtn();
}

document.addEventListener('DOMContentLoaded', init);
</script>
</body>
</html>
)rawliteral";

#endif
