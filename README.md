# tuya-rutx-reporter

A small daemon for Teltonika RUTX routers (OpenWrt / RutOS) that periodically reports basic device information to the Tuya Cloud. It is built on the [Tuya IoT Core SDK for Embedded C](https://github.com/tuya/tuya-iot-core-sdk) and connects over MQTT (TLS).

## What it reports

Every 10 seconds the daemon publishes a property report containing:

| Property            | Description                                                          |
| ------------------- | -------------------------------------------------------------------- |
| `uptime`            | System uptime                                                        |
| `cpu`               | CPU load                                                             |
| `free_ram`          | Free memory                                                          |
| `total_ram`         | Total memory                                                         |
| `network_interfaces`| Name, IPv4 address and netmask of each interface (loopback excluded) |

System and network data is read from `ubus` (`system info` and `network.interface dump`).

The daemon also listens for the `message` action from the cloud and writes the received text to `/tmp/tuya_actions.log`.

## Repository layout

```
tuya-sdk/              # OpenWrt package for the Tuya IoT Core SDK with a shared-library patch
tuya-reporter/         # The daemon (C sources, init script, UCI config)
feeds/vuci/
└── tuya-reporter-api/ # VuCI API support for configuring the service from the web UI
```

## Building

The packages are meant to be built with the OpenWrt / RutOS SDK.

1. Copy the contents of the project into your SDK's `package/` directory.
2. Select `tuya-reporter` (Base system) in `make menuconfig`. The `libtuya-sdk` dependency should be included automatically.
3. Build:

   ```sh
   make package/tuya-reporter/compile
   ```

**Dependencies:** `libtuya-sdk`, `libubus`, `libubox`, `libblobmsg-json`, `argp`.

## Configuration

Create a device in the [Tuya IoT Platform](https://iot.tuya.com/) and copy its credentials into the UCI config which should have been generated during the installation process at `/etc/config/tuya-reporterd`:

```
config tuya-reporterd 'device_sct'
	option device_id 'YOUR_DEVICE_ID'
	option product_id 'YOUR_PRODUCT_ID'
	option secret 'YOUR_DEVICE_SECRET'
	option enable '1'
```

Then start the service:

```sh
/etc/init.d/tuya-reporterd enable
/etc/init.d/tuya-reporterd start
```

The service is managed by `procd` and restarts automatically if it exits. It reloads when the config file changes.

## Running manually

```sh
tuya-reporterd -p <PRODUCT_ID> -d <DEVICE_ID> -s <DEVICE_SECRET> [-D]
```

| Option            | Description                    |
| ----------------- | ------------------------------ |
| `-d, --device`    | Device ID (required)           |
| `-p, --product`   | Product ID (required)          |
| `-s, --secret`    | Device secret (required)       |
| `-D, --daemon`    | Run in the background          |

Logs go to syslog under the `tuya_reporterd` tag:

```sh
logread | grep tuya_reporterd
```
