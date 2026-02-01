Since you are on **Niri** (a Wayland compositor), traditional tools like `xmodmap` will not work. You need a tool that works at the kernel/input level, which is cleaner and works regardless of your window manager.

The best tool for this on Arch Linux is **`keyd`**.

Here is the logic: On a standard AZERTY (ISO) keyboard, the `< >` symbols live on a specific physical key usually located between the Left Shift and Z. This key is technically known as the **"102nd" key**. Since your QWERTY (ANSI) keyboard physically lacks this key, we simply need to tell the computer: *"When I press Caps Lock, pretend I pressed that missing 102nd key."*

### Step 1: Install `keyd`

`keyd` is available in the Arch official repositories.

```bash
sudo pacman -S keyd

```

### Step 2: Create the Configuration

You need to define a configuration file that maps your Caps Lock key to that missing "ISO key" (which AZERTY interprets as chevrons).

1. Create/edit the file `/etc/keyd/default.conf`:
```bash
sudo nano /etc/keyd/default.conf

```


2. Paste the following configuration:
```ini
[ids]
*

[main]
# Map CapsLock to the "102nd" key (The < > key on ISO keyboards)
capslock = 102nd

```



### Step 3: Enable and Reload

Enable the service so it starts on boot and start it now:

```bash
sudo systemctl enable --now keyd

```

### Why this works

* **The Mapping:** You are telling the kernel that the physical `capslock` button is now the `102nd` button.
* **The OS Layout:** Since your OS layout is set to **AZERTY**, it looks at the input from the `102nd` button and produces:
* **Press:** `<` (Less than)
* **Shift + Press:** `>` (Greater than)



### Optional: Keeping "Real" Caps Lock

If you still need the actual Caps Lock function occasionally, you can map it to a combo (like `Ctrl` + `CapsLock`). Update your config like this:

```ini
[main]
capslock = 102nd

# Pressing Control + CapsLock triggers actual CapsLock
[control]
capslock = capslock

```

Reload `keyd` to apply changes: `sudo keyd reload`.

---

**Next Step:** Would you like to know how to set up `capslock` to act as `Esc` when tapped (for Vim) and `Control` when held, while moving the chevrons to a different layer?