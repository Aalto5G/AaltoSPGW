nwEPC Architecture
================

Folder structure
----------------

The folder structure is the following:

```
$ tree -d
.
|-- common
|-- doc
|-- init
|-- nw-event
|   |-- include
|   |-- src
|   `-- test-app
|-- nw-gre
|   |-- include
|   |-- shared
|   |-- src
|   `-- test-app
|-- nw-gtpv1u
|   |-- include
|   |-- shared
|   |-- src
|   `-- test-app
|-- nw-gtpv2c
|   |-- include
|   |-- shared
|   |-- src
|   `-- test-app
|-- nw-ipv4
|   |-- include
|   |-- shared
|   |-- src
|   `-- test-app
|-- nw-mme-emu
|-- nw-sae-gw
|-- nw-sdp
|   |-- include
|   |-- shared
|   |-- src
|   `-- test-app
`-- utils
    |-- nw-gtpu-if
    |-- nw-gtpv2c-if
    |-- nw-ippool
    |-- nw-ipv4-if
    |-- nw-log
    |-- nw-tmr
    `-- nw-udp
```

- `common`: Common libraries used on multiple parts of the source tree

- `doc`: Documentation folder

- `init`: Scripts to start the S-GW, P-GW or SAE-GW daemons with Systemd

- `nw-event`: Asynchronous event functions. Based on libevent.

- `nw-gre`: Generic Routing encapsulation Stack. GRE tunnels are used by PMIP on interface S5/S8. Not used here

- `nw-gtpv1u`: GPRS Tunnel Protocol Stack. Used on interfaces S1-U and S5/S8.

- `nw-gtpv2c`: GPRS Tunnel Protocol version 2, Control protocol. It is used on interfaces S11 and S5/S8.

- `nw-ipv4`: IP version 4 Stack.

- `nw-mme-emu`: Basic MME/eNB emulator.
It emulates both S11 for signaling and S1 interface for userpplane data.
It doesn't support all procedures implemente on the gateways.

- `nw-sae-gw`: S-GW, P-GW and SAE-GW (combined S/P-GW) implementation

- `nw-sdp`: Software Data Plane.
It is the module to defining the user plane tunnel conversions.
The user plane tunnels are unidirectional.
The way they are implemented is with a tunnel structure that converts packets from and to different endpoint technologies: GRE, GTP and IPv4 with the modules nw-gre, nw-gtpv1u and nw-ipv4.
For instance a tunnel GTP to GTP is used to convert traffic from interface S1 to S5.
A GTP to IP or a GRE to IP is used to convert traffic from S5 to SGi.

- `utils`: Util miscelaneous libraries

### Structure of stack modules

As a convention, the headers are separated on include and shared folders.
The headers on include folders are private headers of the module.
The headers on shared folders are public headers to be included when the module is used

The stack architecture builds upon following mentioned entities that are external to it.

- User Layer Protocol (ULP) Entity: This layer implements the intelligent logic for the application and sits on top of the stack.

- Lower Layer Entity: This is the layer below the stack and is responsible for I/O with the stack and network. It will be UDP or L2 depending on the stack. It may or may not be housed in ULP.

- Timer Manager Entity: Timer Manager Entity provides the stack with infrastructure for timer CRUD operations.

- Log Manager Entity: Log Manager Entity provides the stack with callbacks for logging operations. It may or may not be housed in ULP.

The stack modules have a sample applications under 'test-app' directory that can be consulted for usage examples.

On the main header, the API is documented using comments.

### Structure of nw-sae-gw files

- `NwSaeGwUeState*`:

- `NwSaeGwUe`:

- `NwSaeGwUeFsm`:

- `NwSaeGwLog.h` and `NwSaeGwUeLog.h`:

- `NwSaeGwUlp`:

- `NwSaeGwDpe`:

- `NwSaeGwMain.c`:
