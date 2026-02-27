# BeaglePlay Project Presentation

This project was created with the goal of **learning the BeaglePlay board** and
building a **C++ application** using the libraries and capabilities provided by
the platform.

The main motivation behind this project is to **understand and use WebRTC** in an
embedded environment.
Command to build Doxygen stuff -> in the doxygen folder : doxygen Doxyfile
---

## Project Idea

The final and ultimate goal of this project is to implement a **WebRTC-based
application** that:

- Sends an **image** from a PC application (**Peer_A**)  
- Receives the image on the **BeaglePlay board** (**Peer_B**)  
- Processes the image on the BeaglePlay (e.g. increments a variable or modifies metadata)  
- Sends the updated image back to **Peer_A**

![](diagrams/Project_connection_diagram.png)

Both **Peer_A** and **Peer_B** are treated as **WebRTC peers**, not as
client/server entities.

---

## Development Approach

To reach the final goal, the project starts with **very small and simple steps**,
gradually increasing complexity. This allows learning both the hardware and the
software stack in a controlled way.

During development, several technical challenges and learning objectives are
expected, including:

- Inter-thread communication  
- Binary data transfer over WebRTC data channels  
- Serialization and deserialization (using **Protocol Buffers**)  
- Logging and debugging infrastructure  
- Clean project structure and **clear documentation**

---

## Scope

This project is primarily a **learning exercise**, but it is structured as if it
were a real-world embedded application, with emphasis on:

- Code clarity
- Safety
- Maintainability
- Incremental development

