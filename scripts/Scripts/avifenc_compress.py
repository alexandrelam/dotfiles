#!/usr/bin/env python3
import os
import sys
import subprocess
import time
import shutil

# --- CONFIGURATION ---

# File extensions to be treated as photos (will be compressed to AVIF)
PHOTO_EXTENSIONS = ('.jpg', '.jpeg', '.png', '.tiff', '.bmp', '.webp')

# File extensions to be treated as videos (will be copied directly)
VIDEO_EXTENSIONS = ('.mp4', '.mov', '.mkv', '.avi', '.mts', '.webm', '.flv')

# AVIF quality setting for photos. 0-63, where 0 is lossless and 63 is worst.
# A value of 25 is a good balance. Lower is better quality.
AVIF_QUALITY_LEVEL = 25


# --- SCRIPT LOGIC ---

def find_media_files(root_dir):
    """Recursively finds all photo and video files in the given directory."""
    for root, _, files in os.walk(root_dir):
        for file in files:
            lowered_file = file.lower()
            if lowered_file.endswith(PHOTO_EXTENSIONS) or lowered_file.endswith(VIDEO_EXTENSIONS):
                yield os.path.join(root, file)

def get_output_path(input_path, source_dir, dest_dir, new_extension):
    """
    Calculates the new output path, preserving the subdirectory structure.
    Example:
    input_path: /source/photos/vacation/italy.jpg
    source_dir: /source/photos
    dest_dir:   /compressed/media
    new_extension: .avif
    Returns:    /compressed/media/vacation/italy.avif
    """
    # Get the path of the file relative to the source directory
    relative_path = os.path.relpath(input_path, source_dir)

    # Remove the old extension and add the new one
    base, _ = os.path.splitext(relative_path)
    new_relative_path = f"{base}{new_extension}"

    # Join it with the destination directory to get the full output path
    return os.path.join(dest_dir, new_relative_path)


def compress_photo(input_path, source_dir, dest_dir, progress_prefix=""):
    """Compresses a photo to AVIF, saving it to the destination directory."""
    output_path = get_output_path(input_path, source_dir, dest_dir, ".avif")

    # Create the destination subdirectory if it doesn't exist
    os.makedirs(os.path.dirname(output_path), exist_ok=True)

    if os.path.exists(output_path):
        print(f"{progress_prefix}-> Skipping, AVIF already exists: {os.path.basename(output_path)}")
        return False # Indicates that no new work was done

    print(f"{progress_prefix}🖼️  Compressing Photo: {os.path.basename(input_path)} -> .../{os.path.relpath(output_path, dest_dir)}")

    command = [
        'avifenc',
        '-q', str(AVIF_QUALITY_LEVEL),
        '--speed', '4', # Speed preset 0 (slowest/best) to 10 (fastest/worst)
        input_path,
        output_path
    ]

    try:
        subprocess.run(command, check=True, stdout=subprocess.DEVNULL, stderr=subprocess.PIPE)
        print(f"✅ Success: Saved to destination folder.")
        return True # Indicates success
    except FileNotFoundError:
        print("\nERROR: 'avifenc' command not found. Please install 'libavif-tools'.")
        sys.exit(1)
    except subprocess.CalledProcessError as e:
        print(f"❌ Failed to compress {input_path}.")
        print(f"Error: {e.stderr.decode('utf-8')}")
        # Clean up partially created file on failure
        if os.path.exists(output_path):
            os.remove(output_path)
        return False


def copy_video(input_path, source_dir, dest_dir, progress_prefix=""):
    """Copies a video file to the destination directory, preserving its path."""
    # Get the original extension to preserve it
    _, extension = os.path.splitext(input_path)
    output_path = get_output_path(input_path, source_dir, dest_dir, extension)

    # Create the destination subdirectory if it doesn't exist
    os.makedirs(os.path.dirname(output_path), exist_ok=True)

    if os.path.exists(output_path):
        # To be safe, check if file size is the same. If not, re-copy.
        try:
            if os.path.getsize(output_path) == os.path.getsize(input_path):
                print(f"{progress_prefix}-> Skipping, file already exists and size matches: {os.path.basename(output_path)}")
                return False # Indicates that no new work was done
        except OSError:
            # If size check fails, proceed to copy
            pass

    print(f"{progress_prefix}🎬 Copying Video: {os.path.basename(input_path)} -> .../{os.path.relpath(output_path, dest_dir)}")

    try:
        # copy2 preserves file metadata like creation/modification time
        shutil.copy2(input_path, output_path)
        print(f"✅ Success: Copied to destination folder.")
        return True # Indicates success
    except Exception as e:
        print(f"❌ Failed to copy {input_path}.")
        print(f"Error: {e}")
        # Clean up partially created file on failure
        if os.path.exists(output_path):
            os.remove(output_path)
        return False

def main():
    """Main function to parse arguments and start the process."""
    if len(sys.argv) != 3:
        print("Usage: ./compress_and_copy_media.py <source_directory> <destination_directory>")
        print("Example: ./compress_and_copy_media.py '~/Pictures' '~/Processed Pictures'")
        sys.exit(1)

    source_dir = os.path.expanduser(sys.argv[1])
    dest_dir = os.path.expanduser(sys.argv[2])

    if not os.path.isdir(source_dir):
        print(f"Error: The source directory does not exist: {source_dir}")
        sys.exit(1)

    if os.path.abspath(source_dir) == os.path.abspath(dest_dir):
        print("Error: Source and destination directories cannot be the same.")
        sys.exit(1)

    os.makedirs(dest_dir, exist_ok=True)

    print("--- Starting Media Processing ---")
    print("This script will COMPRESS photos to AVIF and COPY videos as-is.")
    print("✨ If the script is stopped, you can run it again to resume.")
    print(f"Source Directory:      {source_dir}")
    print(f"Destination Directory: {dest_dir}")
    print(f"Photo Quality (AVIF CQ): {AVIF_QUALITY_LEVEL} (Lower is better)")
    print("---------------------------------\n")

    # --- Collect and sort all files first to ensure a predictable order ---
    print("🔎 Finding all media files... ", end="", flush=True)
    all_files_to_process = sorted(list(find_media_files(source_dir)))
    total_files = len(all_files_to_process)
    if total_files == 0:
        print("No media files found to process. Exiting.")
        sys.exit(0)
    print(f"Found {total_files} files.")
    print("---------------------------------\n")

    # SAFETY WARNING
    print("⚠️  WARNING: This script creates new files in the destination folder.")
    print("⚠️  Original files are NOT deleted. The source folder remains untouched.\n")

    # Confirmation prompt
    try:
        if sys.stdout.isatty():
            confirm = input("Do you want to continue? (y/n): ")
            if confirm.lower() != 'y':
                print("Operation cancelled.")
                sys.exit(0)
    except EOFError:
        print("Running in non-interactive mode. Proceeding automatically.")
        pass

    start_time = time.monotonic()
    files_processed_this_run = 0

    for i, file_path in enumerate(all_files_to_process, 1):
        progress = f"[{i}/{total_files}] "
        lowered_path = file_path.lower()

        work_done = False
        if lowered_path.endswith(PHOTO_EXTENSIONS):
            work_done = compress_photo(file_path, source_dir, dest_dir, progress)
        elif lowered_path.endswith(VIDEO_EXTENSIONS):
            work_done = copy_video(file_path, source_dir, dest_dir, progress)

        if work_done:
            files_processed_this_run += 1

    end_time = time.monotonic()
    duration = end_time - start_time
    minutes, seconds = divmod(duration, 60)

    print("\n--- Processing Complete ---")
    print(f"Scanned a total of {total_files} media files.")
    print(f"Processed {files_processed_this_run} new files in this run.")

    if int(minutes) > 0:
        print(f"Total time taken for this run: {int(minutes)} minute(s) and {seconds:.2f} seconds.")
    else:
        print(f"Total time taken for this run: {seconds:.2f} seconds.")


if __name__ == "__main__":
    main()
