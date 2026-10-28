import os
from PIL import Image

def next_power_of_2(x):
    """Calculates the next power of 2 for a given integer."""
    return 1 if x == 0 else 1 << (x - 1).bit_length()

def process_sprites(directory):
    if not os.path.exists(directory):
        print(f"Directory not found: {directory}")
        return

    for filename in os.listdir(directory):
        filepath = os.path.join(directory, filename)

        # Skip subdirectories if any exist
        if os.path.isdir(filepath):
            continue

        # Delete non-PNG files
        if not filename.lower().endswith('.png'):
            print(f"Deleting non-PNG file: {filename}")
            os.remove(filepath)
            continue

        # Process PNG files
        try:
            with Image.open(filepath) as img:
                # Ensure image has an alpha channel
                img = img.convert("RGBA")
                old_w, old_h = img.size
                
                new_w = next_power_of_2(old_w)
                new_h = next_power_of_2(old_h)
                
                # Skip if the image is already perfectly sized
                if old_w == new_w and old_h == new_h:
                    print(f"Skipping {filename}: already {old_w}x{old_h}")
                    continue
                    
                print(f"Padding {filename} from {old_w}x{old_h} to {new_w}x{new_h}")
                
                # Create a new transparent power-of-2 canvas
                new_img = Image.new("RGBA", (new_w, new_h), (0, 0, 0, 0))
                
                # Calculate coordinates to center the original image
                offset_x = (new_w - old_w) // 2
                offset_y = (new_h - old_h) // 2
                
                # Paste the original image onto the center of the new canvas
                new_img.paste(img, (offset_x, offset_y))
                
            # Overwrite the original file with the new padded version
            new_img.save(filepath, format="PNG")
            
        except Exception as e:
            print(f"Failed to process {filename}: {e}")

if __name__ == "__main__":
    target_dir = os.path.join(".", "romfs", "sprites")
    process_sprites(target_dir)
    print("Sprite processing complete.")