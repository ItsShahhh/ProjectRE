# ProjectRE (Project Resident Evil)

Inspired by Resident Evil Which an Horror/Shooter A Short Project Showcase Created By Me Focusing On Simple and Complex Mechanics Such As Inventory, Parkour, Weapon, Enemy AI, etc.
_Noted This Also My First Ever C++ Project and i learned a lot of new ways to code_

---

## Weapon Mechanics

<img width="800" height="400" alt="WeaponRecoil" src="https://github.com/user-attachments/assets/704be8a8-f37a-49c2-b7fa-846657fe020d" />

**Overview:** Line Trace for shooting weapon will be based on each weapon bullet pallet
**Technical Implementation:**
- Inside of weapon base it will calculation will be made using for each loop referencing each weapon bullet pallet float (https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.cpp#L88-L152)
- Based on the numbers of the bullet pallet weapon will shoot that amount https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.h#L67-L68
- Each bullet pallet will also been given random spread radius that also be calculated inside for each loop https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.cpp#L88-L152


**Overview:** Adding Weapon Recoil by calculating current camera recoil and adding new weapon recoil to AddControllerYawInput
**Technical Implementation:**
- Include Weapon Recoil Force float variable inside weapon base https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.h#L58-L59
- Inside event tick Current Weapon Recoil will get override by new Weapon Recoil Force and Last Weapon Recoil will saved the old Current Weapon Recoil    https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/ProjectRECharacter.cpp#L242-L256
- On event tick also calculated between all those floats and added it to AddControllerYawInput https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/ProjectRECharacter.cpp#L242-L256

## Player Mechanics

### Interactable

_Note:_ The first two seconds of the gif is interactable with IK off and the rest of the clip is with IK on
<img width="1152" height="594" alt="IKPickableItem" src="https://github.com/user-attachments/assets/0e37186c-54f9-4fa2-90c5-a0acd3f19043" />

**Overview:** Interact with pickable will activate IK that move hand to the location of the interact pickable item 
**Technical Implementation:**
- Line Trace will shoot that will check if the pickable object type is World Static and implement UInteractableInterface https://github.com/ItsShahhh/ProjectRE/blob/255f34115a3fa9d4f972917236253e76c6885680/Source/ProjectRE/InteractableTrace.cpp#L32-L56
- Player need to face the interact item to be able to pick it up https://github.com/ItsShahhh/ProjectRE/blob/255f34115a3fa9d4f972917236253e76c6885680/Source/ProjectRE/InteractableTrace.cpp#L58-L85


### Vaulting

_Note:_ This is vaulting with IK hand off
<img width="800" height="400" alt="NoIKVaulting" src="https://github.com/user-attachments/assets/3e2c753e-98e0-475a-87ec-28466ddac0d5" />

_Note:_ This is vaulting with IK hand on
<img width="800" height="400" alt="IKVaulting" src="https://github.com/user-attachments/assets/fca2d753-67e3-46ce-a4c8-3a88ad043578" />

**Overview:** Multi stage raycasting approach combined with root motion animations to ensure smooth, natural interaction with varied geometry
**Technical Implementation:**
- Line Trace will be cast using actor forward vector and multiply it by vault distance, the vault trace will be subtract so it will be cast down https://github.com/ItsShahhh/ProjectRE/blob/d05b57d797c41d81a0abfd83b35152dbe9a3f20f/Source/ProjectRE/VaultObstacle.cpp#L72-L75
- Forward sphere trace will checks if an vaulting location is block by obstacle using overlapping https://github.com/ItsShahhh/ProjectRE/blob/d05b57d797c41d81a0abfd83b35152dbe9a3f20f/Source/ProjectRE/VaultObstacle.cpp#L91-L106
- IK hand will be activated based on curve values and certain frame in animation, it will also fire an line trace to the hand bone to get the exact location of hand (This is inside Vaulting Animation for AnimNotify) 
- Vaulting animation is also using motion warping to make sure it always landed on given location and doesn't clip through obstacle (Motion Warping Notify is used inside the Vaulting Animation)

### Mantle

<img width="800" height="400" alt="Mantling" src="https://github.com/user-attachments/assets/0d6a7986-a3e2-46c6-8d35-0230331ca6b3" />

**Overview:** A multi-stage raycasting approach combined with Motion Warping and root motion animations to seamlessly scale walls, high ledges, and elevated geometry
**Technical Implementation:**
- A forward line trace casts at head level using the actor’s forward vector multiplied by the mantle range. If this trace hits an obstacle, the wall is determined to be too tall, instantly failing the mantle attempt. https://github.com/ItsShahhh/ProjectRE/blob/d05b57d797c41d81a0abfd83b35152dbe9a3f20f/Source/ProjectRE/VaultObstacle.cpp#L108-L126
- If the head-level check passes, a secondary line trace casts forward with an added Z-axis offset above the ledge. The resulting impact point identifies the top surface, providing the exact target vector passed into the Motion Warping system. https://github.com/ItsShahhh/ProjectRE/blob/d05b57d797c41d81a0abfd83b35152dbe9a3f20f/Source/ProjectRE/VaultObstacle.cpp#L48-L68
- Motion Warping notifies embedded in the mantle animation dynamically adjust the root motion path to warp the character cleanly onto the calculated surface target without clipping through geometry or floating in mid-air. https://github.com/ItsShahhh/ProjectRE/blob/d05b57d797c41d81a0abfd83b35152dbe9a3f20f/Source/ProjectRE/ProjectRECharacter.cpp#L674-L687

### Through Tight Spaces

<img width="800" height="400" alt="ThroughTightSpace" src="https://github.com/user-attachments/assets/1dd2ecbc-dc27-4d8f-b2ad-401a1101b717" />

**Overview:** A dedicated tight-space traversal system that aligns the player, locks movement to a single axis, dynamically scales movement animations based on character speed, and manages smooth camera transitions into and out of the squeeze zone.
**Technical Implementation:**
- When entering the tight space, the system uses an Arrow Component attached to the entry point to snap the player's position and rotate them so they face directly toward the wall https://github.com/ItsShahhh/ProjectRE/blob/eb21e961da956b99d88b17368ed3939c75e7f5ca/Source/ProjectRE/ThroughTightSpaces.cpp#L40-L74
- While traversing inside the space, movement input is strictly restricted to forward and backward motion along the defined traversal path, preventing lateral or diagonal movement https://github.com/ItsShahhh/ProjectRE/blob/eb21e961da956b99d88b17368ed3939c75e7f5ca/Source/ProjectRE/ThroughTightSpaces.cpp#L103-L112
- The playback rate of the squeeze animation dynamically scales based on the character's real-time velocity vector, ensuring the animation speed matches actual movement speed without foot sliding or static play (This function is inside player animation blueprint)
- Upon entry, the system smoothly blends the active player camera to a dedicated fixed/constrained camera component located on the tight-space actor, providing an optimized cinematic framing during traversal https://github.com/ItsShahhh/ProjectRE/blob/eb21e961da956b99d88b17368ed3939c75e7f5ca/Source/ProjectRE/ProjectRECharacter.cpp#L615-L621

### Climbing Ladder

<img width="800" height="400" alt="ClimbingLadder" src="https://github.com/user-attachments/assets/a742736d-703d-4990-9431-b26127c53eff" />

**Overview:** A vertical ladder climbing system that locks player movement to a single upward axis and dynamically scales the climbing animation playback rate based on movement speed
**Technical Implementation:**
- Upon interacting with or mounting the ladder, player movement input is strictly locked to the vertical axis, allowing only upward (and downward) progression while suppressing lateral movement https://github.com/ItsShahhh/ProjectRE/blob/eb21e961da956b99d88b17368ed3939c75e7f5ca/Source/ProjectRE/ClimbingLadder.cpp#L71-L75
- The playback rate of the ladder-climbing animation dynamically scales relative to the character’s actual vertical velocity vector, ensuring step animations synchronize precisely with movement speed without foot slipping (This function is inside player animation blueprint) 
- Reaching the top or bottom exit threshold of the ladder releases the vertical axis lock, restoring standard movement controls and returning the character to their default locomotion state https://github.com/ItsShahhh/ProjectRE/blob/eb21e961da956b99d88b17368ed3939c75e7f5ca/Source/ProjectRE/ClimbingLadder.cpp#L77-L97

### Unlock Door

<img width="800" height="400" alt="UnlockDoor" src="https://github.com/user-attachments/assets/560ac8ab-482c-4392-aa85-bc26541068de" />

**Overview:** An interactive door lock system that blends to a close-up camera, displays an inventory-driven key selection UI, checks key compatibility, triggers contextual audio feedback, and activates physics-based door movement upon successful unlock
**Technical Implementation:**
- Interacting with a locked door smoothly blends the player camera to a dedicated lock view component and displays a UI showing the player's available key inventory https://github.com/ItsShahhh/ProjectRE/blob/eb21e961da956b99d88b17368ed3939c75e7f5ca/Source/ProjectRE/ProjectRECharacter.cpp#L718-L730
- Selecting an inventory slot checks if a key is present and compares its key Gameplay Tags against the door actor's required key variable (This function is inside the UI Unlock Door Slot)
- Plays a distinct sound effect based on the verification outcome (failure sound for an incorrect key or missing slot, success sound for a match) https://github.com/ItsShahhh/ProjectRE/blob/eb21e961da956b99d88b17368ed3939c75e7f5ca/Source/ProjectRE/UnlockDoor.cpp#L68-L77
- Matching the required key unlocks the door and enables Simulate Physics on its physics constraint, allowing it to swing open realistically https://github.com/ItsShahhh/ProjectRE/blob/eb21e961da956b99d88b17368ed3939c75e7f5ca/Source/ProjectRE/UnlockDoor.cpp#L65-L66

### Getting Hit

<img width="800" height="400" alt="PlayerHit" src="https://github.com/user-attachments/assets/7e7540a0-c4e8-4b6e-9a92-aa1631e0d7ef" />

**Overview:** A dynamic HUD damage feedback system that displays screen-space blood splatter UI animations upon receiving damage, triggering persistent pulse transitions and heartbeat audio cues when character health drops below a critical threshold
**Technical Implementation:**
- Receiving damage triggers a transient blood splatter widget overlay on the player screen, playing a timed UI animation sequence that auto-hides upon completion https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/Health.cpp#L33-L49
- When below critical health, the blood splatter UI overrides its standard fade-out sequence to execute a looping ping-pong (fade in/out) opacity animation that persists while health remains low (This function is inside the Health UI)
- Concurrently triggers a persistent heartbeat sound effect https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/ProjectRECharacter.cpp#L882-L894

## Enemy Mechanics

### Enemy Stumble

<img width="800" height="400" alt="EnemyStumbleHit" src="https://github.com/user-attachments/assets/5ba42f27-56e4-494a-826f-ed629739818d" />

**Overview:** A localized physical reaction system that dynamically enables physics on hit skeletal bones to simulate realistic bullet impact force, then seamlessly blends the character mesh back into its original animated pose via a recovery timeline while filtering out invalid bone targets
**Technical Implementation:**
- Weapon line traces detect impact locations and extract the specific hit bone name, checking against a parent bone filter to ensure non-simulating or critical skeletal bones are excluded from physical reaction
- Validated hit bones dynamically trigger Simulate Physics, applying directional impulse force to simulate realistic body movement and stumble at the exact point of impact
- Following a brief delay, a Blueprint timeline executes to gradually dial down physics simulation and smoothly blend the affected bones back to their original animation pose without popping

### Enemy Dismemberment

_Noted:_ One Arm Dismember
<img width="800" height="400" alt="OneHandDismemberment" src="https://github.com/user-attachments/assets/e5e6905d-cde2-496b-adce-615acd5c6a7d" />

_Noted:_ Two Arm Dismember
<img width="800" height="400" alt="TwoHandDismemberment" src="https://github.com/user-attachments/assets/886a1a68-f24a-4a7d-8278-ac31489f798b" />

**Overview:** A dynamic limb dismemberment system that hides/removes target bones upon hit detection, spawns a physical skeletal/static ragdoll piece at the socket location to simulate realistic severance, and dynamically adjusts the enemy’s attack animation set based on remaining functional limbs
**Technical Implementation:**
- Weapon line traces detect impacts to arm bones, triggering a targeted mesh operation to hide the specific limb bone down to zero, visually severing it from the main skeletal mesh https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/EnemyBase.cpp#L207
- Spawns a separate limb mesh actor at the exact transform of the removed bone, applying impulse forces and enabling physics simulation to create a realistic detached body part https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/EnemyBase.cpp#L208-L216
- Tracks the enemy's active limb count and updates internal combat state flags to dynamically switch combat montages and attack animations appropriate for their remaining limbs https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/EnemyBase.cpp#L218-L228

### Enemy Death

<img width="800" height="400" alt="EnemyDeath" src="https://github.com/user-attachments/assets/c112a4ce-f346-4a89-9be3-2ed965f39cb1" />

**Overview:** An enemy death visual effect system that dynamically swaps materials to a custom dissolve shader upon death, utilizing a timeline to drive scalar parameters across all material slots for a smooth, organic disappearance
**Technical Implementation:** 
- Upon receiving a death event, the enemy character replaces its active material instances on the skeletal mesh with a dedicated dissolve material template https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/EnemyBase.cpp#L131-L146
- A custom Blueprint timeline triggers immediately after the swap, gradually updating the dissolve scalar parameter value from start to finish across every material index https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/EnemyBase.cpp#L148-L153
- The scalar parameter controls the opacity mask or cutout threshold within the shader, creating a progressive, realistic dissolve effect across the entire mesh before destroying or pooling the actor (This scalar parameter is inside the Materials)

### Enemy Blood Splatter

## Game Mechanics

### Quest System

<img width="800" height="400" alt="QuestSystem" src="https://github.com/user-attachments/assets/21a5cd19-b55d-42ac-a3b3-ae00430582be" />

**Overview:** Data Asset-driven Quest System that displays customizable objective text on the player's HUD, tracks progression via Actor-bound Quest Subsystems, formats dynamic variable progress strings, and handles sequential quest chaining upon objective completion
**Technical Implementation**
- When a quest activates, the system loads a customizable Quest Data Asset and broadcasts its localized objective text to the Quest UI for screen display https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/QuestSubsystem.cpp#L20-L42
- Quest-relevant actors (e.g., enemies, collectable keys) house a Quest Subsystem component that listens for interactions or completion triggers and reports completed events back to the manager https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/QuestSubsystem.cpp#L44-L64
- Objective text utilizes FText::Format with positional format arguments ({}), dynamically inserting runtime variables (such as current kill counts or item counts) directly into the string layout as progress updates
  [Inside Objective Base] https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/QuestObjectiveBase.h#L29-L30
  [Child Objective] https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/KillQuestObjective.cpp#L21-L27
- Upon receiving an event completion signal, the system evaluates objective requirements and automatically retrieves and activates the next quest Data Asset specified in the current asset's chain configuration https://github.com/ItsShahhh/ProjectRE/blob/fa66285906b01f3d634eff645efab202a91b92e4/Source/ProjectRE/QuestSubsystem.cpp#L66-L87

### Breakable Barrel

<img width="800" height="400" alt="BreakableBarrel" src="https://github.com/user-attachments/assets/e9a27462-ecd9-4a2a-b55c-8572586be50b" />

**Overview:** An interactive breakable barrel system that hides its intact mesh upon impact, spawns a loot item selected at random from a table alongside a broken barrel mesh variant, and uses a delayed timer to handle clean actor garbage collection.
**Technical Implementation** 
- Upon receiving an impact callback (OnReceiveImpact), checks for valid loot configuration maps (AmmoSpawnActor) and a broken mesh template (BrokenBarrelActor), aborting with debug logs if missing
- Instantly hides the intact BarrelMesh and disables its collision (NoCollision), preserving the actor instance to safely run internal cleanup logic instead of destroying it immediately
- Selects a random AAmmoBase subclass key from the AmmoSpawnActor collection, offsets the Z-axis coordinate by AmmoSpawnHeight to avoid floor clipping, and spawns the item into the world
- Spawns a physical BrokenBarrelActor at the original transform, then executes a Blueprint-safe lambda timer (SetTimer) after BrokenBarrelLifetime seconds to destroy both the spawned debris and the barrel actor itself

## User Widget (UI)

<img width="800" height="400" alt="ShowingAmmoUI" src="https://github.com/user-attachments/assets/4a33c092-82c6-4a60-8c96-744c3b766e40" />

**Overview:** Aiming Weapon will show max ammo and current ammo at the side of weapon
**Technical Implementation:**
- User Widget for showing ammo will be visible aiming an weapon https://github.com/ItsShahhh/ProjectRE/blob/e680f3df1ca6a13eba11d90f97bae90d8d63ea70/Source/ProjectRE/WeaponBase.cpp#L283
- Based on the current ammo it will updated on the Ammo User Widget using event dispatcher
  
[Broadcast for checking ammo when shooting weapon]
https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.cpp#L110
[Broadcast for checking ammo when reload weapon]
https://github.com/ItsShahhh/ProjectRE/blob/050738574ea28a686832097c88f345907e0ac403/Source/ProjectRE/WeaponBase.cpp#L257


<img width="400" height="400" alt="InventoryItem" src="https://github.com/user-attachments/assets/e9f615c4-cf7c-45b9-b891-fd28ad843af6" />

**Overview:** Stackable item inventory system and can replace unoccupied item slot with new item on the same slot
**Technical Implementation:**
- Inventory is refresh/checks every time inventory is open https://github.com/ItsShahhh/ProjectRE/blob/93dbef5f9bd8794a8c38a90650c3149a3b8292e9/Source/ProjectRE/InventoryComponent.cpp#L149-L174
- Check will happen inside of an array for any potential item that meet the requirement (Which is Gameplay Tag and Empty Slot) to be added to the inventory array https://github.com/ItsShahhh/ProjectRE/blob/93dbef5f9bd8794a8c38a90650c3149a3b8292e9/Source/ProjectRE/InventoryComponent.cpp#L158-L166
- Event Dispatcher will be broadcast at the end of array to inventory user widget https://github.com/ItsShahhh/ProjectRE/blob/93dbef5f9bd8794a8c38a90650c3149a3b8292e9/Source/ProjectRE/InventoryComponent.cpp#L169
- Same type of ammo inside inventory is stackable


<img width="400" height="400" alt="HotbarWeaponSlots" src="https://github.com/user-attachments/assets/e7829fe2-3948-49eb-9a76-55a9c88bbe44" />

**Overview:** Only Weapon is added onto the hotbar and can be move from slots to slots
**Technical Implementation:**
- Picking up weapon will automatically added to hotbar array https://github.com/ItsShahhh/ProjectRE/blob/93dbef5f9bd8794a8c38a90650c3149a3b8292e9/Source/ProjectRE/InventoryComponent.cpp#L63
- Dragged on hotbar slot and it will check that slot for texture then attach it to mouse cursor (This function is inside blueprint)
- When Dropped onto another hotbar slot that already occupied with weapon (Which is Not Empty) it revert back to previous dragged slot (This function is inside blueprint)
