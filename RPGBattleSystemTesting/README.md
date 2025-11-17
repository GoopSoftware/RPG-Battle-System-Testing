# Oldschool RPG (C++ / Raylib)

This project is an **oldschool turn-based RPG** built in **C++** using the **Raylib** graphics library.  
The game uses an **ECS (Entity Component System)** architecture to keep gameplay logic modular, scalable, and easy to extend.

---

## Architecture Overview

### **Entity Component System (ECS)**
All gameplay logic is organized around entities (game objects), components (data), and systems (logic).  
Systems operate only on entities that have the components they need.

---

## Systems Overview

### **GameStateManager**
- Always running — controls global game flow.  
- Handles **state transitions** between:
  - `Overworld`
  - `Battle`
  - `Menu`
  - `GameOver`
- Maintains the active state and delegates update loops to the appropriate system.

---

### **OverworldSystem**
- Always live but **only updates when the GameStateManager is in the Overworld state**.
- Responsibilities:
  - Player movement and interaction  
  - Random encounter rolls  
  - Random encounter creation  
  - Maintains **environment context** (e.g., Forest, Ocean, Fire Temple) to influence enemy selection and encounter difficulty
- Returns an `Encounter` struct to the `GameStateManager` containing:
  - Entity IDs for enemies  
  - Context data (environment, difficulty, etc.)

---

### **BattleSystem**
- Created dynamically **when an encounter begins** and destroyed when it ends.  
- Constructed with:
  - Active player entities  
  - Encounter data from `OverworldSystem`  
  - Component stores (player health, stats, names, etc.)
- Runs the **turn-based battle loop**, managing:
  - Turn order  
  - Player and enemy actions  
  - Victory/defeat checks  
- Returns a **flag** (`Victory` or `Defeat`) to the `GameStateManager`.

---

### **RenderSystem**


---

## Game Flow Example

1. **Overworld Phase**
   - Player moves freely  
   - A random encounter roll is made each step  
   - On success, an encounter is created and passed to `GameStateManager`  
   - `GameStateManager` switches the state to **Battle**

2. **Battle Phase**
   - A new `BattleSystem` instance is constructed with overworld data  
   - The battle loop runs until victory or defeat  
   - On **Victory** → return to **Overworld**  
   - On **Defeat** → switch to **Game Over**

3. **Game Over Phase**
   - Displays `"You Died"`  
   - Handles restart or quit logic

---

## Current Work
- Expanding **encounter generation** logic (location + difficulty scaling)  
- Adding **environment-specific enemy pools**, inspired by *Final Fantasy VIII*-style scaling

---

## Planned Additions
- UI Rendering system (menus, HUD, transitions)  
- Save/Load system  
- NPC interaction and dialogue system  
- Audio management system (music + SFX)  
- Dynamic environment tilesets

---

## Design Philosophy
> “Keep it modular, keep it simple.”  
The goal is to emulate the charm of classic JRPGs with modern development practices — a clean ECS structure, smooth transitions, and replayable systems.

---

## Controls (Prototype)
| Action | Key |
|--------|-----|
| Move Up | W / ↑ |
| Move Down | S / ↓ |
| Move Left | A / ← |
| Move Right | D / → |
| Confirm | Enter |
| Cancel | Q |

---

## Dependencies
- **C++17 or higher**  
- **Raylib** (https://www.raylib.com/)  

---

## Author
**DirtyZeusLover**  
Built with passion for oldschool RPGs and ECS-based architecture.  
