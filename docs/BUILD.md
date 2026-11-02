# Building MISSA (development)

The initial Python reference modules can be tested with:

```sh
python3 -m unittest discover -s tests -v
```

Firmware integration uses upstream [fm1-base](https://github.com/charlesvestal/fm1-base).
The verified application contract is `include/plat.h` plus `template/app.h`: `app_render`, `app_init`, `app_ui_init`, `app_frame`, `app_input_only`, `app_panic`, `app_say`. The LCD is 240×240. Physical input is `plat_buttons()`, `plat_enc()`, `plat_keys()`. Persistent data is `plat_store_load/save`.

Firmware compilation requires a native upstream checkout and its documented SDK/toolchain; it has **not yet been executed**. No .fwsc image has been produced. The work is a validated **reference-engine foundation**, not yet a flashable firmware.
