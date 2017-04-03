nwEPC - EPC Gateways
====================

This software provides a C implementation of S-GW, P-GW or a combined version of the both SAE-GW. It is a fork of the nwEPC provided by Amit Chawre (original [Readme](README-old))


Dependencies
------------

This software has been tested on Ubuntu 16.04 and previously on Ubuntu 14.04. It should work on any linux system.
The only dependency is libevent.
Systemd is recomended and multiple init scripts are provided.
To build the binary, additional tools are required.


### Build

```
# apt-get install build-essential m4 libtool autoconf libevent-dev
```


### Install

```
# apt-get install libevent-2.0-5
```


Build & installation process
----------------------------

The build process follows the normal autotools workflow

```
$ libtoolize -i
$ autoreconf -i
$ ./configure
$ make
$ sudo make install
```

Configuration
-------------

To installation adds the necessary unit files to configure the services as deamons in Systemd, this files are:

### Files involved on configuration

Folder `/etc/systemd/network/` :
- `sgi0.netdev` : tun device definition for SGi interface
- `sgi0.network` : network configuration for SGi interface


Folder `/lib/systemd/system/` :
- `sgw.service`  : unit describing the S-GW daemon
- `pgw.service`  : unit describing the P-GW daemon
- `spgw.service` : unit describing the S/P-GW daemon


Folder `/etc/conf.d/iproute2/` :
- `pgw.iproute2.start` : P-GW configuration setup
- `pgw.iproute2.stop`  : P-GW configuration clean


Folder `/etc/cumucore/` :
- `sgw.cfg.template`  : configuration template for S-GW
- `pgw.cfg.template`  : configuration template for P-GW
- `spgw.cfg.template` : configuration template for S/P-GW


The configuration of the services is done on the configuration files located on `/etc/cumucore/`. This are the only files a user should modify. Fill the configuration files and remove the template extention.

### Systemd workflow

- After installing the software, make sure systemd reloads the new unit files:

```
# systemctl daemon-reload
```

- Reload the network to add the new devices:

```
# systemctl restart systemd-networkd
```

- Enable the required services sgw|pgw|spgw to start them on boot , i.e.

```
# systemctl enable sgw
```

- Start the service

```
# systemctl start sgw|pgw|spgw
```

- Check the logs

```
# journalctl -n100f -u sgw|pgw|spgw
```

Interfaces to be configured for each node
-----------------------------------------

S-GW:
- S1 (GTP-U)
- S11
- S5/S8

P-GW:
- S5/S8 user plane (GTP-U)
- S5/S8 controll plane,
- SGi with all the PDN parameters (APN, UE IP subnet, DNS, MTU, etc.)

S/P-GW: all the previous ones
