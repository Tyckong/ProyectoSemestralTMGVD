from collections import Counter
import sys
import os

def process_file(filepath, output_folder):          #Processes a single file to calculate character frequencies.
    with open(filepath, 'r') as file:
        count = Counter(list(file.read()))
        sz = sum(count.values())

    os.makedirs(output_folder, exist_ok=True)
    output_path = os.path.join(output_folder, f"reduced_{os.path.basename(filepath)}.txt")

    with open(output_path, 'w') as reduced:
        for k, v in count.items():
            reduced.write(f"{ord(k)} {v / sz}\n")

def process_folder(folder_path):    #Processes all files in a given folder.
    for root, _, files in os.walk(folder_path):     #Create an equivalent output folder structure
        relative_path = os.path.relpath(root, folder_path)      #Path relative to the input folder
        output_folder = os.path.join("reduced_Genomes", relative_path)      #Output mirrors the input structure

        for file in files:
            filepath = os.path.join(root, file)
            print(f"Processing: {filepath}")
            process_file(filepath, output_folder)

if __name__ == '__main__':
    argv = sys.argv
    if len(argv) != 2:
        print('Usage: python3 char_freq.py <FOLDER>')
    else:
        input_path = argv[1]
        if os.path.isfile(input_path):
            print("Error: Input path must be a folder.")
        elif os.path.isdir(input_path):
            process_folder(input_path)
