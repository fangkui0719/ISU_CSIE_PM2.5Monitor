# Introduction

- The department of Computer Science and Information Engineering in I-Shou University, Taiwan.

- A course in IoT Wireless Transmission.

- Pragram language : C / C++, Javascript, Node.js

- Tools : Arduino IDE, Visual Studio, NODE-RED, MediaTek Cloud Sandbox ( MCS ), Twilio

- Device : LinkIt 7697, Dragino LoRa Shield, UpBoard

# Architecture

+--------------+   +------+   +---------------+   +------------+                     
| PM2.5 Sensor |---| LoRa |-->| LoRa (Server) |---| LinkIt7697 |--> MCS --> | UpBoard (NODE-RED) | --> e-mail, SMS, Android app
+--------------+   +------+   +---------------+   +------------+     |
                                                                     --------------------------------> Website


# Program

- LoRa_PM_Sensor : Collect PM2.5 information and send to Server.

- LoRa_PM_Server : Receive data from sersor and transfer to gateway.

- LinkIt7697_Gateway : Upload data to MediaTek Cloud Sandbox (MCS).

# Author

- 邱方奎

- 蔡宗恩

- 張哲誌

- 趙彥廷

- 朱洺鋐

