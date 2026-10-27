import os
from PIL import Image


def convert_to_raw(input_dir, output_dir):
  for root, dirs, files in os.walk(input_dir):
    for file in files:
      if file.lower().endswith(".png"):
        in_path = os.path.join(root, file)
        with Image.open(in_path) as img:
          # Convert to RGB/BGR format matching 3DS framebuffers
          img = img.convert("RGB")
          # 3DS screen framebuffers are vertical (240x400 for top screen)
          # Resize or process as needed, then save raw bytes
          raw_data = img.tobytes("raw", "RGBA")

          rel_path = os.path.relpath(root, input_dir)
          out_subdir = os.path.join(output_dir, rel_path)
          os.makedirs(out_subdir, exist_ok=True)

          out_path = os.path.join(
              out_subdir, os.path.splitext(file)[0] + ".raw"
          )
          with open(out_path, "wb") as f:
            f.write(raw_data)
  print("Conversion complete!")


if __name__ == "__main__":
  convert_to_raw("./romfs/sprites", "./romfs/sprites_raw")