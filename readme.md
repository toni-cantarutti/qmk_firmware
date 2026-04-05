# ✨ Custom QMK firmware for Keychron K2 Pro ANSI RGB

<p align="center">
  <img alt="Keyboard" src="https://img.shields.io/badge/Keyboard-Keychron%20K2%20Pro-2563eb?style=for-the-badge">
  <img alt="Firmware" src="https://img.shields.io/badge/Firmware-QMK-111827?style=for-the-badge">
  <img alt="Layout" src="https://img.shields.io/badge/Layout-QWERTY--fr-16a34a?style=for-the-badge">
  <img alt="Modes" src="https://img.shields.io/badge/Modes-Mac%20%2F%20Windows-f59e0b?style=for-the-badge">
</p>

<p align="center">
  A custom <strong>QMK firmware</strong> for the <strong>Keychron K2 Pro</strong>, designed for
  <strong>development</strong>, <strong>French typing</strong>, and <strong>fast text navigation</strong>.
</p>

> [!NOTE]
> This firmware is built around **two selectable layouts** accessed through the keyboard's physical **Mac / Windows** switch. In practice, the switch is used here more like **layout 1 / layout 2** than as a strict operating-system selector: one layout enables the **smart space bar**, while the other keeps a **fully standard space bar** with normal hold and repeat behavior.

---

## 📚 Table of contents

- [1. What this custom firmware is for](#1-what-this-custom-firmware-is-for)
  - [Why this firmware exists](#-why-this-firmware-exists)
  - [Design philosophy](#-design-philosophy)
  - [QWERTY-fr, Right Alt, and why this firmware relies on it](#-qwerty-fr-right-alt-and-why-this-firmware-relies-on-it)
  - [Layer overview](#-layer-overview)
  - [Important remap: Caps Lock becomes Right Alt](#-important-remap-caps-lock-becomes-right-alt)
  - [How the special space bar works in Windows mode](#-how-the-special-space-bar-works-in-windows-mode)
  - [Shortcuts available when holding Space in Windows mode](#-shortcuts-available-when-holding-space-in-windows-mode)
  - [Why these keys were chosen](#-why-these-keys-were-chosen)
  - [Fn layer](#-fn-layer)
  - [Intended difference between Mac mode and Windows mode](#-intended-difference-between-mac-mode-and-windows-mode)
  - [Compatibility notes](#-compatibility-notes)
  - [Implementation details](#-implementation-details)
  - [Practical summary](#-practical-summary)
  - [Source of truth](#-source-of-truth)
- [2. Installation, build, and flashing](#2-installation-build-and-flashing)
  - [Assumptions](#-assumptions)
  - [Full procedure](#-full-procedure)
  - [Expected result after compilation](#-expected-result-after-compilation)
  - [Which file to use](#-which-file-to-use)
  - [Flashing with QMK Toolbox](#-flashing-with-qmk-toolbox)
  - [Quick summary](#-quick-summary)

---

## 1. What this custom firmware is for

This repository contains a custom QMK firmware for a **Keychron K2 Pro**.

Its goal is to provide **two selectable keyboard layouts** through the keyboard's physical **Mac / Windows switch**, with a strong focus on:

- working smoothly with the **QWERTY-fr** system layout
- making French accented characters easier to access through **Right Alt / AltGr**
- reducing hand movement by turning the space bar into a navigation key when held
- keeping a second layout available where the space bar behaves like a completely normal key again

In practice, this firmware does **not** treat the switch primarily as a real **Mac mode / Windows mode** distinction.

Instead, the switch is used more like this:

- **layout 1**: a layout with the custom space-bar behavior for navigation and editing
- **layout 2**: a more standard layout where the space bar behaves normally again, including normal hold and repeat behavior

The physical labels on the keyboard still say **Mac** and **Windows**, but in this setup they are mainly just two convenient hardware positions for switching between those two layouts.

### Why this firmware exists

This firmware was built to solve a very specific set of needs:

1. Keep the benefits of a **QWERTY keyboard** for coding, shortcuts, and cross-platform use.
2. Still type French comfortably by relying on a **QWERTY-fr** system layout, where accented characters and other French symbols are typically accessed through **Right Alt / AltGr** combinations.
3. Reduce right-hand travel while editing text by turning common letter keys into arrows, editing keys, and symbol shortcuts when the space bar is held.
4. Keep a second, more standard layout available for situations where a completely normal space bar is preferable, for example gaming or any workflow where regular key repeat matters.

Rather than treating the keyboard as "one OS mode for Mac and one OS mode for Windows", this firmware uses the keyboard's **hardware Mac / Windows switch** mainly as a simple selector between **two layouts with two different space-bar philosophies**.

---

### Design philosophy

| Switch position | Practical role in this firmware | Key idea |
|---|---|---|
| **Mac** | Standard layout | Normal space bar, including hold and repeat |
| **Windows** | Navigation layout | Smart space bar + home-row shortcuts |

#### Mac position on the switch

In this firmware, the **Mac** position is used as the **standard layout**.

- The space bar remains a **real, standard space bar**.
- Typing stays close to stock keyboard behavior.
- Holding space behaves normally, including repeat.
- This is useful whenever you want the keyboard to feel conventional again.

That can be preferable for things like gaming, or any situation where the smart-space behavior is not desirable.

#### Windows position on the switch

In this firmware, the **Windows** position is used as the **navigation and text-editing layout**.

- The space bar becomes a custom dual-role key.
- `I J K L` become arrow keys while space is held.
- Other keys become `Home`, `End`, `Backspace`, `Delete`, or `Right Alt` combinations.
- This makes navigation and QWERTY-fr character entry much easier without leaving the home area.

The result is a compact thumb-driven shortcut system built around the space bar.

> [!TIP]
> The labels **Mac** and **Windows** should mostly be read here as the names of the switch positions, not as a strict statement about which operating system must be used.

---

### QWERTY-fr, Right Alt, and why this firmware relies on it

A key idea behind this firmware is that it does **not** hardcode accented characters directly in QMK.

Instead, it mainly sends combinations such as:

- `Right Alt + key`
- in one specific case, `Ctrl + Alt + C`

This matters because the design is split across two layers of responsibility:

- **the firmware** handles gestures and shortcut ergonomics
- **the operating system** handles the final character output through the active keyboard layout

If the active system layout is **QWERTY-fr**, those `Right Alt` combinations can produce accented letters, cedillas, French punctuation, or other special characters depending on the OS implementation.

**Why this is useful:**

- it preserves the familiar **QWERTY base layer** for coding and standard shortcuts
- it keeps French character logic at the **OS layout level**, which is easier to reuse across Windows, macOS, and Linux
- it avoids maintaining a large OS-specific symbol table inside QMK itself

> [!TIP]
> This firmware is designed to **work with QWERTY-fr**, not replace it.

---

### Layer overview

The firmware defines four layers:

- `MAC_BASE`
- `MAC_FN`
- `WIN_BASE`
- `WIN_FN`

These names come from QMK and from the keyboard's physical switch labels, but in practice they map to **two user-facing layouts**:

| Layer | Practical role | Notes |
|---|---|---|
| `MAC_BASE` | Standard base layout | Standard `KC_SPC`, `Caps Lock` replaced by `Right Alt` |
| `MAC_FN` | Standard layout Fn layer | Secondary functions |
| `WIN_BASE` | Navigation base layout | Uses custom `NAV_SPC` |
| `WIN_FN` | Navigation layout Fn layer | Secondary functions |

#### `MAC_BASE`

Main typing layer for the **standard layout**.

- Standard space bar with `KC_SPC`
- `Caps Lock` replaced by `Right Alt`
- Layout kept intentionally close to normal behavior

#### `MAC_FN`

Utility layer used with the standard layout.

#### `WIN_BASE`

Main typing layer for the **navigation layout**.

- Space bar replaced by the custom `NAV_SPC` keycode
- `Caps Lock` replaced by `Right Alt`
- Navigation and `Right Alt` output available while holding space

#### `WIN_FN`

Utility layer used with the navigation layout.

The two Fn layers are mostly aligned so the keyboard stays easy to understand.

---

### Important remap: Caps Lock becomes Right Alt

On both base layers, the key in the **Caps Lock position** is remapped to `Right Alt`.

This is one of the most practical changes in the whole layout.

**Why do this?**

- It makes **AltGr-style combinations** much easier to type.
- It brings French character entry closer to the center of the keyboard.
- It gives much faster access to QWERTY-fr symbols than relying only on the bottom-right `Right Alt` key.

So this firmware effectively gives you **two physical ways to press `Right Alt`**:

- the normal bottom-right `Right Alt`
- the old `Caps Lock` position

That is especially useful when your OS layout is QWERTY-fr and accented characters depend heavily on `Right Alt` / `AltGr`.

---

### How the special space bar works in Windows mode

The custom keycode `NAV_SPC` is the core feature of the navigation layout, which in this firmware is selected through the **Windows** position of the physical switch.

| Action | Result |
|---|---|
| **Tap** `Space` | Sends a normal space |
| **Hold** `Space` + another key | Activates navigation / editing / symbol behavior |

#### Tap behavior

If space is pressed and released by itself, it sends a normal space.

#### Hold behavior

If space is held while another key is pressed, it becomes a **navigation / editing / symbol modifier**.

You can think of it as a small shortcut layer under your thumb.

> [!IMPORTANT]
> In Windows mode, the space bar is **not** a real held `KC_SPC`.

Instead, the behavior is implemented in software:

- if used alone, the firmware sends a tapped space
- if used with another key, the firmware switches into shortcut mode

That means auto-repeat behavior for space is **not exactly the same** as with a plain hardware `KC_SPC`.

In Mac mode, the space bar is a normal `KC_SPC`, so it behaves like a standard keyboard key.

---

### Shortcuts available when holding Space in Windows mode

#### Navigation

| Shortcut | Output |
|---|---|
| `Space + I` | `Up` |
| `Space + J` | `Left` |
| `Space + K` | `Down` |
| `Space + L` | `Right` |

#### Line navigation

| Shortcut | Output |
|---|---|
| `Space + U` | `Home` |
| `Space + O` | `End` |

#### Text editing

| Shortcut | Output |
|---|---|
| `Space + H` | `Backspace` |
| `Space + ;` | `Delete` |

#### `Right Alt` output

The following combinations send `Right Alt + key`:

- `Space + E`
- `Space + D`
- `Space + 2`
- `Space + 3`
- `Space + 4`
- `Space + W`
- `Space + Q`
- `Space + A`
- `Space + F`

These combinations do not have a universal meaning at the firmware level. The final character depends on the keyboard layout active in the operating system, especially if you are using a **QWERTY-fr** layout.

#### Special case for `C`

- `Space + C` sends `Ctrl + Alt + C`
- however, if `Ctrl` is already being held, the firmware lets normal `C` pass through

This exception exists so that **`Ctrl + C`** still works normally and is not broken by the space-based shortcut system.

The choice of `Ctrl + Alt + C` also makes sense on Windows, where **AltGr is commonly interpreted as `Ctrl + Alt`**.

---

### Why these keys were chosen

The key choices are not arbitrary.

#### `I J K L` for arrows

These keys form a compact directional block that is easy to memorize and sits naturally under the right hand.

#### `U / O` for `Home / End`

These keys sit close to the arrow cluster and work well as start-of-line and end-of-line actions.

#### `H / ;` for `Backspace / Delete`

These make corrections fast without moving toward the edges of the keyboard.

#### `E D 2 3 4 W Q A F` with `Right Alt`

These shortcuts bring useful **QWERTY-fr `Right Alt` outputs** under the thumb-based space system, instead of forcing you to reach for the normal bottom-right `Right Alt` every time.

In practice, this firmware speeds up two things at once:

- **navigation**
- **entry of French or special characters that depend on `AltGr` / `Right Alt`**

---

### Fn layer

The `MAC_FN` and `WIN_FN` layers are mainly utility layers.

They include:

- `UG_TOGG` on `Fn + Esc` for keyboard lighting toggle
- `F15` to `F24`
- `Task View`
- `Calculator`
- media controls
- volume controls
- Bluetooth host switching
- battery level

The goal is not to make Fn complicated. It is simply a clean place for secondary functions.

---

### Intended difference between Mac mode and Windows mode

This firmware is **not** trying to create a strict Mac-versus-Windows operating-system split.

Instead, it intentionally creates a split between **two layouts with different behavior**.

| In the standard layout | In the navigation layout |
|---|---|
| more predictable behavior | smart space bar |
| normal space bar with normal repeat | home-row-friendly navigation |
| fewer custom typing rules | easier access to `Right Alt` combinations for French typing |
| useful when you want something close to stock behavior | layout optimized for text editing and shortcut-heavy workflows |

This asymmetry is deliberate. One layout stays simple and conventional, while the other becomes the advanced productivity layout.

---

### Compatibility notes

#### 1. Accent output depends on the OS layout

The firmware does not generate accented letters by itself. It sends key combinations.

So for the expected French characters to appear, your computer must use a compatible system layout such as **QWERTY-fr** or another layout that maps those `Right Alt` combinations the way you expect.

#### 2. The advanced behavior lives on the switch's Windows position

The custom space logic is only enabled on the `WIN_BASE` layer.

In this firmware, that corresponds to the **Windows** position of the physical switch, but this should be understood as the **navigation layout**, not as a strict requirement about the operating system you are using.

#### 3. The switch's Mac position keeps the standard space behavior

On the `MAC_BASE` layer, the space bar stays a normal `KC_SPC`.

In this firmware, that position is effectively the **standard layout**, useful whenever you want normal typing behavior back, including regular space hold and repeat.

#### 4. Space repeat is different in the navigation layout

This is expected, because the navigation-layout space key is implemented through the custom `NAV_SPC` logic rather than a raw `KC_SPC` hold.

---

### Implementation details

The `keymap.c` file relies on a few small but important pieces of logic.

#### `NAV_SPC`

This is the custom keycode used instead of a standard space bar on the Windows base layer.

#### `nav_space_down` and `nav_space_used`

These booleans are used to distinguish between:

- a simple press of the space bar
- a space hold that was used as a modifier for another key

#### `handle_nav_hold(...)`

This helper function handles the hold behavior for navigation and editing keys. It turns letter keys into their target actions while `NAV_SPC` is active.

#### `tap_with_ralt(...)`

This helper function sends `Right Alt + key`. It is used for the shortcuts that rely on the QWERTY-fr system layout.

---

### Practical summary

#### What this firmware adds

- a **standard layout** that stays simple and close to normal behavior
- a **navigation layout** built around the smart space bar
- a **smart space bar** for navigation and editing
- easier access to **Right Alt / AltGr**
- better integration with the **QWERTY-fr** system layout
- useful but readable Fn layers
- a fast way to switch back to a normal space bar when needed

#### In one sentence

This firmware turns the K2 Pro into a **QWERTY keyboard optimized for development, French typing, and fast text navigation**, while also keeping a **second, more standard layout** available whenever you want the space bar to behave normally again.

---

### Source of truth

The actual firmware behavior is defined in `keymap.c`.

This README documents the intent, design, and everyday usage, but if there is ever a doubt about a specific shortcut or edge case, **the code is the source of truth**.

---

## 2. Installation, build, and flashing

This section explains how to set up QMK on a fresh Linux machine, clone this fork, build the custom firmware, find the correct output file, and flash the keyboard with QMK Toolbox.

### Assumptions

- GitHub fork: `toni-cantarutti/qmk_firmware`
- branch containing the custom firmware: `k2pro-custom`
- keymap to compile: `custom`
- keyboard target: `keychron/k2_pro/ansi/rgb`
- the commands below assume that the QMK repo is cloned directly into the current directory

---

### Full procedure

#### Step 0. Move to the final directory

This directory will become the root of the `qmk_firmware` repository, so it should be empty.

```bash
mkdir -p ~/firmware/qmk_firmware
cd ~/firmware/qmk_firmware
```

#### Step 1. Install QMK CLI

```bash
curl -fsSL https://install.qmk.fm | sh
```

#### Step 2. Reload `.bashrc`

```bash
source ~/.bashrc
```

#### Step 3. Verify that QMK CLI is installed correctly

```bash
qmk --version
```

#### Step 4. Clone the fork into the current directory

When QMK asks `Would you like to clone ... ?`, press `Y` then `Enter`.

```bash
qmk setup -H . toni-cantarutti/qmk_firmware
```

#### Step 5. Switch explicitly to the branch that contains the custom firmware

```bash
git checkout k2pro-custom
```

#### Step 6. Update submodules after switching branches

The `k2pro-custom` branch updates submodule pointers, so refresh them after the checkout.

```bash
git submodule update --init --recursive
```

#### Step 7. Check the current branch

```bash
git branch --show-current
```

#### Step 8. Check the environment

```bash
qmk doctor
```

#### Step 9. Build the custom firmware

```bash
qmk compile -kb keychron/k2_pro/ansi/rgb -km custom
```

---

### Expected result after compilation

After a successful build, QMK generates build artifacts in the `.build/` directory.

You should normally see output similar to this:

```text
Creating binary load file for flashing: .build/keychron_k2_pro_ansi_rgb_custom.bin [OK]
Creating load file for flashing: .build/keychron_k2_pro_ansi_rgb_custom.hex [OK]
Copying keychron_k2_pro_ansi_rgb_custom.bin to qmk_firmware folder
```

---

### Which file to use

The firmware file to use for flashing is:

```text
keychron_k2_pro_ansi_rgb_custom.bin
```

QMK usually leaves it in two places:

1. in the build directory:

   ```text
   .build/keychron_k2_pro_ansi_rgb_custom.bin
   ```

2. at the root of the `qmk_firmware` repo:

   ```text
   ./keychron_k2_pro_ansi_rgb_custom.bin
   ```

> [!IMPORTANT]
> The file to use in **QMK Toolbox** is the **`.bin`** file.

Do not use:

- `.elf`
- `.exe`

A `.hex` file may also exist, but in this setup the practical file to load into QMK Toolbox is:

```text
./keychron_k2_pro_ansi_rgb_custom.bin
```

---

### Flashing with QMK Toolbox

#### 1) Open QMK Toolbox

Launch **QMK Toolbox**.

#### 2) Load the firmware

In QMK Toolbox:

- click **Open**
- select this file:

```text
keychron_k2_pro_ansi_rgb_custom.bin
```

#### 3) Put the keyboard into flash mode

For the **Keychron K2 Pro**, turn the keyboard **off**, then turn it back **on while holding the Escape key**.

In other words:

1. switch the keyboard to **Off**
2. hold **Esc**
3. turn the keyboard back on
4. release **Esc**

The keyboard should then enter bootloader / flash mode.

#### 4) Flash the keyboard

Once the keyboard is detected by QMK Toolbox:

- make sure the correct `.bin` file is loaded
- click **Flash**

#### 5) Verify

When the flashing process completes, the keyboard should reboot with the custom firmware.

---

### Quick summary

- **Build command**

  ```bash
  qmk compile -kb keychron/k2_pro/ansi/rgb -km custom
  ```

- **Firmware file to flash**

  ```text
  keychron_k2_pro_ansi_rgb_custom.bin
  ```

- **K2 Pro flash mode**

  ```text
  Off -> hold Esc -> turn it back on -> Flash in QMK Toolbox
  ```
