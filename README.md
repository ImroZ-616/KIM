# KIM
KIM (Kernel Interface Module) — Wrist-adjacent AI assistant on ESP32. Talks to a locally-hosted LLM over Wi-Fi for private, offline terminal access to your laptop. No cloud required.

# Roadmap: ESP32 Device ↔ Laptop ↔ Local AI Model

A step-by-step learning path for building a Wi-Fi-connected ESP32 device (buttons + display) that talks to a locally-hosted AI model on your laptop.

---

## 1. Core Fundamentals

Before touching Wi-Fi or AI, you need the two legs this project stands on: **networking** and **embedded hardware**.

**Networking basics to study:**
- IP addresses, ports, and how two devices "find" each other on the same network
- The client-server model (who initiates, who responds)
- TCP vs UDP — TCP for reliable ordered data (what you'll mostly use), UDP for speed-over-reliability
- What a "local network" means — why your ESP32 and laptop need to be on the same Wi-Fi/subnet to talk directly
- DNS vs raw IP (you'll likely just use your laptop's local IP, e.g. `192.168.1.x`)

**Microcontroller basics to study:**
- What ESP32 actually is: a microcontroller with built-in Wi-Fi/Bluetooth, GPIO pins, limited RAM/flash compared to a laptop
- Difference between microcontroller programming and normal app programming: no OS (or a tiny RTOS), everything runs in a loop, memory is precious
- GPIO (General Purpose Input/Output) — how buttons are read as digital signals
- How a small display (SSD1306 OLED, ST7735 TFT, etc.) is driven — usually via I2C or SPI protocol
- Power and stability basics — Wi-Fi radios draw noticeable current, which matters later for the wearable stage

**Toolchain to get comfortable with:**
- Arduino IDE or PlatformIO (PlatformIO is more powerful once projects grow)
- ESP-IDF (Espressif's native framework) — optional now, but worth knowing it exists for later, more advanced control

**How this connects to your project:** Everything downstream depends on this. The ESP32 reads a button (GPIO), needs to send that as a message over Wi-Fi (networking), and later draw a response on the display (I2C/SPI). If these fundamentals are shaky, debugging the "advanced" stuff later becomes guesswork.

---

## 2. How ESP32 Communicates Over Wi-Fi

Once the ESP32 is on your network, it needs a *protocol* — an agreed-upon format for sending/receiving messages.

**Concepts to learn:**
- **HTTP request-response model**: ESP32 sends a request (e.g. "here's my button press"), server sends back a response (e.g. AI's reply). Simple, stateless, easy to debug.
- **WebSockets**: a persistent, two-way connection. Once opened, either side can send data anytime without re-asking. Better for real-time, chat-like interactions.
- **REST API concepts**: endpoints, methods (GET/POST), status codes, JSON as the data format
- **Client-server roles in your specific project**: the ESP32 is almost always the *client* (it initiates), your laptop server is the *server* (it listens and responds)

**Keywords to search/study:**
- "ESP32 HTTPClient library" (for HTTP)
- "ESP32 WebSockets library (arduinoWebSockets)"
- "JSON parsing on ESP32 (ArduinoJson library)"
- "ESP32 Wi-Fi station mode vs access point mode" (you want *station mode* — ESP32 joins your existing Wi-Fi)

**How to decide HTTP vs WebSockets for you:** Start with HTTP. It's simpler to reason about and debug (you can even test your server with a browser or Postman before the ESP32 is involved). Move to WebSockets later if you want lower latency or a more "live" feel (see Section 6).

**How this connects:** This is the wire between your button press and your AI model. Get this rock-solid before adding AI complexity — test it by just echoing back whatever the ESP32 sends, no AI involved yet.

---

## 3. Building a Local Server on Your Laptop

Your laptop needs a program that listens for ESP32 requests, does something (call the AI), and replies.

**Concepts to learn:**
- What a "server" actually is: a program that binds to a port and listens for incoming connections
- Routing — mapping URL paths to functions (e.g. `/ask` triggers your AI-calling logic)
- Request/response handling — parsing incoming JSON, constructing outgoing JSON
- Running a server on your local network so devices *other than* your laptop can reach it (binding to `0.0.0.0` instead of `localhost`, firewall considerations)
- Basic concurrency — what happens if the ESP32 sends a request while a previous one is still processing (important once AI response times are slow)

**Tools/frameworks to consider (pick one, don't overthink it):**
- **FastAPI** (Python) — modern, has built-in support for WebSockets too, great docs, good for someone with your Python background
- Flask (Python) — simpler, older, huge community, slightly more manual for WebSockets
- Node.js + Express — if you'd rather work in JS

Given your existing Python/OOP background, **FastAPI** is a strong fit — it also naturally teaches you async programming, which matters once you're calling a (slow) AI model.

**Keywords to study:**
- "REST API basics"
- "FastAPI quickstart" / "Flask quickstart"
- "async/await in Python" (for handling slow AI calls without freezing the server)
- "CORS" (only relevant if you ever add a browser-based UI alongside the ESP32)

**How this connects:** This server is the "brain's receptionist" — it takes the ESP32's request, forwards it to the AI, and relays the answer back. Build and test this piece using just a browser or `curl`/Postman before your ESP32 ever gets involved.

---

## 4. Connecting the Server to a Local AI Model

Now the server needs to actually generate intelligent responses.

**Concepts to learn:**
- What "running a model locally" means — the model weights and inference engine run on your laptop's CPU/GPU, no internet or cloud API needed
- The idea of an inference server — Ollama/llama.cpp expose *their own* local API (usually HTTP) that your FastAPI/Flask server can call, just like it would call any external API
- Prompt construction — how you format the text you send to the model (system prompt, user prompt, conversation history)
- Model size vs speed vs quality tradeoffs — smaller quantized models run faster on modest hardware but are less capable

**Tools/platforms to study:**
- **Ollama** — easiest starting point, runs models like Llama 3, Mistral, Phi, etc. with a simple local HTTP API (`localhost:11434`)
- **llama.cpp** — lower-level, more control, more setup effort
- Quantization concepts (e.g. Q4, Q8) — why a "7B Q4" model is smaller/faster but slightly less accurate than full precision

**Keywords to search:**
- "Ollama REST API"
- "local LLM inference"
- "GGUF model format"
- "prompt engineering basics"

**How this connects:** Your FastAPI server becomes a middleman: ESP32 → your server → Ollama's local API → your server → ESP32. Test this link independently too — call Ollama directly from a terminal or Postman before wiring it into your ESP32 flow.

---

## 5. Data Flow: Device → Server → AI → Back to Device

This is where you connect Sections 2–4 into one working pipeline. Think of it as a relay race.

**The full round trip:**
1. Button press on ESP32 → ESP32 constructs a request (e.g. JSON: `{"query": "..."}`)
2. Request sent over Wi-Fi to your laptop's server (HTTP POST or WebSocket message)
3. Server receives it, extracts the query
4. Server sends that query to Ollama's local API
5. Ollama returns generated text
6. Server packages that text as JSON and sends it back to the ESP32
7. ESP32 parses the response and renders it on the small display

**Concepts to learn:**
- Serialization/deserialization (turning data into JSON and back) at *both* ends — ArduinoJson on the ESP32 side, native JSON handling in Python
- Timeout handling — what happens if the AI takes 10+ seconds to respond? Your ESP32 shouldn't just hang forever
- Error handling at each hop — Wi-Fi drops, server crashes, AI model errors — each needs a graceful fallback message

**How this connects:** Build and test each hop *in isolation* first (as suggested in earlier sections), then chain them. This is the single biggest time-saver: don't wire all four pieces together and debug blind — you'll never know which link broke.

---

## 6. Real-Time Communication vs Request-Response

This decision shapes your whole architecture, so it's worth understanding deeply rather than picking arbitrarily.

**Request-response (HTTP):**
- ESP32 asks, waits, gets one answer, connection closes
- Simple mental model, easy to debug, stateless
- Downside: no way for the server to "push" something to the ESP32 unprompted; also has more per-request overhead

**Real-time (WebSockets):**
- Connection stays open; either side can send anytime
- Better for things like streaming AI tokens as they're generated (so your display could show text appearing word-by-word instead of waiting for the full response)
- More complex — you need to manage connection state, reconnects if Wi-Fi hiccups, etc.

**Concepts to study:**
- Streaming responses (most local AI APIs support token-by-token streaming, not just an all-at-once reply)
- Connection lifecycle management (connect, disconnect, reconnect logic)
- Polling as a middle-ground alternative (ESP32 periodically asks "any update?") — simpler than WebSockets but less elegant

**How this connects, and what to actually pick for you:** Start with request-response HTTP for your first working version. Once that's solid, WebSockets are a natural "v2" upgrade, especially valuable if you later want streaming text on the display (feels more alive) or a wearable that needs to stay "always connected" without constantly re-negotiating a new HTTP connection each time (saves battery too).

---

## 7. UI/UX for a Small Embedded Display

Designing for a 128x64 OLED is a completely different discipline than designing for a laptop screen.

**Concepts to learn:**
- Text wrapping and pagination — AI responses are often long; your display isn't. You need to decide: scroll? Paginate with button presses? Truncate?
- Font size vs information density tradeoffs
- Using icons/symbols instead of full words where possible (saves space, faster to parse at a glance)
- State indication — showing "thinking...", "connected", "error" states clearly, since the user can't see your server logs
- Button-driven navigation patterns — since you likely have few buttons, think in terms of short-press/long-press, or a simple menu system

**Tools/libraries to know about (conceptually, not code):**
- Adafruit GFX / U8g2 — common graphics libraries for small displays, support fonts, basic shapes, bitmaps
- Bitmap/icon design tools for tiny embedded icons

**How this connects:** This is the "last mile" of your user experience. A technically perfect pipeline feels broken if the user can't tell whether the device is thinking, connected, or has failed. Plan your display states (idle / listening / sending / waiting for AI / showing answer / error) as a simple state diagram before writing any display code.

---

## 8. Debugging Techniques for This Kind of System

Multi-device systems fail in more places than single-program apps, so your debugging approach needs to be layered.

**Techniques to build up:**
- **Serial monitor logging** on the ESP32 — your first and most important debugging tool; print every step (button pressed, Wi-Fi connected, request sent, response received)
- **Testing each layer independently** (as emphasized above): test the server alone with Postman/curl, test Ollama alone via terminal, test ESP32 networking alone by hitting a dummy "echo" endpoint
- **Network-level debugging**: tools like `ping` to check the laptop is reachable, checking both devices are actually on the same subnet, checking laptop firewall isn't blocking the port
- **Server-side logging**: print incoming requests, outgoing AI calls, and response times — this tells you where time is being spent if things feel slow
- **Common failure categories** to learn to recognize: Wi-Fi connectivity issues, malformed JSON, server not reachable (IP/firewall), AI model timeout, display rendering bugs — each has a distinct "signature" once you've seen it a few times

**Keywords to study:**
- "ESP32 Serial.println debugging"
- "curl basics" / "Postman for API testing"
- "HTTP status codes" (so you can read what a failed request is actually telling you)

**How this connects:** Given you're integrating hardware + networking + AI, assume something *will* break at each boundary at least once. Building in logging at every hop from day one will save you far more time than it costs.

---

## 9. Scaling This Toward a Wearable (Wrist Assistant)

Once your desktop-tethered prototype works, moving to a wearable form factor introduces a new set of constraints.

**New concepts to learn:**
- **Power management**: battery selection (LiPo), charging circuits (TP4056 or similar), and — critically — Wi-Fi's power draw versus lower-power alternatives like Bluetooth Low Energy (BLE) for shorter-range communication
- **Deep sleep / light sleep modes** on ESP32 — waking only on button press instead of always listening, to conserve battery
- **Form factor constraints**: smaller displays, fewer buttons, possibly touch or gesture input instead
- **Connectivity range**: Wi-Fi range is a real limitation for "walk around the house" use; you may eventually explore whether your laptop server needs to be reachable over your home network more broadly, or whether a lighter BLE-to-phone-to-server bridge makes more sense
- **Alternative architectures**: phone-as-bridge (wearable ↔ BLE ↔ phone app ↔ Wi-Fi/internet ↔ server) is a common pattern for real wearables, since phones handle connectivity and power far better than a tiny wearable can alone

**Keywords to study:**
- "ESP32 deep sleep power consumption"
- "BLE vs Wi-Fi power comparison"
- "wearable electronics battery management"
- "ESP32-C3 / ESP32-S3" (newer, smaller, more power-efficient variants worth knowing about for this stage)

**How this connects:** Don't design for the wearable from day one — it'll slow down your learning. Get the desktop-tethered version fully working first, since all the AI/networking/UI logic transfers directly. The wearable stage is primarily an electrical engineering and power-budget problem layered on top of software you'll have already proven works.

---

## Suggested Build Order (Summary)

1. Get ESP32 on Wi-Fi, blink an LED or read a button, print to Serial Monitor
2. Get a display showing static text
3. Build a bare FastAPI server on your laptop, test with a browser/Postman
4. Get ESP32 sending an HTTP request to that server and displaying the raw response
5. Install Ollama, test it standalone via terminal
6. Wire FastAPI → Ollama, test via Postman (no ESP32 yet)
7. Connect the full chain: ESP32 → FastAPI → Ollama → FastAPI → ESP32 → display
8. Polish UI states (thinking/error/idle) and add logging throughout
9. (Optional, later) Upgrade to WebSockets for streaming responses
10. (Optional, much later) Redesign for battery + wearable form factor

Each numbered step above should be a small, testable milestone — resist the urge to jump straight to step 7.