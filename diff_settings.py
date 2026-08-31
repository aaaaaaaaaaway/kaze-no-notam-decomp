def apply(config, args):
    config["arch"] = "mipsel"
    config["baseimg"] = "baserom.bin"
    config["myimg"] = "build/payload.bin"
    config["mapfile"] = "build/kaze.map"
    config["source_directories"] = ["src", "include"]
    config["objdump_executable"] = "mipsel-linux-gnu-objdump"
    config["map_format"] = "gnu"
