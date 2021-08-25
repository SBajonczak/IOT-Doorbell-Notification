![image](https://github.com/SBajonczak/hivemonitor/workflows/PlatformIO%20CI/badge.svg)
![image](https://img.shields.io/github/v/tag/SBajonczak/hivemonitor.svg)

## :ledger: Index

- [About](#beginner-about)
- [Technologies](#zap-technologies)
  - [Configured Platforms](#configured_platforms)
  - [Development](#package-development)
  - [Pre-Requisites](#notebook-pre-Requisites)
  - [Branches](#cactus-branches)
- [Development Environment](#nut_and_bolt-development_environment)
  - [Build the Firmware](#hammer-build_the_firmware)
  - [Upload firmware](#rocket-upload_the__firmware)
  - [Upload the configuration](#page_facing_up-upload_the_configuration)  
- [Hardwaresetup](#electric_plug-hardwaresetup)
  - [Components List](#components_List)
  - [Wiring Schema](#wiring_schema)
  - [Wiring H30A to HX711](#wiring_h30a_to_hx711)
- [Configuration](#configuration)
  - [Upload_Configuration](#upload_configuration)
- [Planned Features](#planned_features)
# :beginner: About
In my homeoffice I wear always ma headphones. So I sometimes didn't realize that my doorbell rings. 

So I decided, to connect an ESP Device to my doorbell. This will trigger the esp up from it's sleep and send me a small message to my broker. 
So that I will get an notification onto my desktop.

# Technologies
This project will not work alone, so it has dependencies. 

## Configured Platforms
Actually it was Build and Testet on the following device

* ESP 8266 Devboard

## :package: Development


## :notebook:Pre-Requisites

* bblanchon/ArduinoJson@^6.18.3
* bblanchon/StreamUtils@^1.6.1
* knolleary/PubSubClient@^2.8

## :cactus: Branches
Here some description about the used branches

|Branch|Description|
|-|-|
|master|This contains the latest stable version|
|development|In this branch, I will put every development work for now. This branch is __NOT__ stable|


# :nut_and_bolt: Development Environment
First of all, the follwing commands required an installation of Platform IO. You can install it with the follwoing command: 

```bash
setup-virtualenv
```

## :hammer: Build the Firmware
You can Build your firmware very easyly with the following command:

```bash
make build-complete
```


## :rocket: Upload the firmware
After a successfull build you can upload it to your connected device with: 

```bash
make upload-firmware
```
In some cases you have multiple device connected. In this case, you must set the port to upload before with this command:

```bash
export PLATFORMIO_UPLOAD_PORT=/dev/ttyUSB0
```

This upload the firmware throught the /dev/ttyUSB0 port.

## :page_facing_up: Upload the configuration
```bash
export PLATFORMIO_UPLOAD_PORT=/dev/ttyUSB0
make upload-config
```

# :electric_plug:  Hardwaresetup 
The following section will describe the hardware construction

## Components List
I Ordered the following parts from my local seller

TBD

## Wiring Schema
 TBD

# Configuration 
The Configuration is done with an json file. An example of it looks like this:

```json
{
   
    "wifi": {
        "ssid": "",
        "password": "33"
    },
    "mqtt": {
        "server": "kjkhj",
        "port": 0,
        "user": "null",
        "password": "null"
    }

}

You can set and read the configuration when you use the maintenance mode.

```
The following table will give you more insights about the settings.

|Group|Setting|Description|
|-|-|-|
|wifi|ssid|The wifi SSID.|
|wifi|password|The password to authenticate.|
|mqtt|server|The host for the mqtt. This can be a dns name or a ip address.|
|mqtt|port|The Mqtt Port.|
|mqtt|user|The username when authentication is configured.|
|mqtt|password|The password when authentication is configured.|



## Upload Predefined Configuration
After you modified the configuration to your setting, you can upload this to your device.
```bash
 make upload-config
```

# Planned Features

Nothing planed at the moment
# FAQ

# My device does not start the AP mode
In this case you must erase the complete flash memmory of your device. 
You can do it in platform io with: 

```
pio run --target erase 
```