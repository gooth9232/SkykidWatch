#ifndef SETTINGS_H
#define SETTINGS_H

//NTP Settings
#define NTP_SERVER "pool.ntp.org"
#define GMT_OFFSET_SEC 3600 * 9 //Tokyo

watchySettings settings{
    .ntpServer = NTP_SERVER,
    .gmtOffset = GMT_OFFSET_SEC,
    .vibrateOClock = false,
};

#endif