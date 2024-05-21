# Network Simulator

This project is developed by implementing functionalities at different layers of the network stack. It allows the creation of end devices, hubs, switches, and routers, enabling the establishment of network topologies. The simulator supports data transmission, address learning, access control, flow control, routing protocols, and process-to-process communication, providing a comprehensive understanding of network protocols and their interactions.

## Demo Video

<iframe width="560" height="315" src="https://www.youtube.com/embed/your_video_id?autoplay=1" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Layers Implemented

1. Physical layer
2. Data Link layer
3. Network layer
4. Transport layer
5. Application layer

## Protocols Implemented

1. Stop and Wait ARQ
2. Selective Repeat
3. Token Passing
4. RIP (Routing Information Protocol)
5. HTTP
6. DNS

## Assumption

The network simulator is designed solely for the purpose of simulating network functionalities, and therefore, no real data is exchanged or transmitted during its operation.

## Language Used

C++17

## Dependencies

1. `cstdlib.h`
2. `chrono.h`
3. `unistd.h`
4. `random.h`

## Code Execution

1. Compile the code using a C++ compiler. You can use the GNU Compiler Collection (GCC) by running the following command:
    ```sh
    g++ main.cpp -o output
    ```
2. Execute the generated executable by running the following command:
    ```sh
    ./output
    ```
