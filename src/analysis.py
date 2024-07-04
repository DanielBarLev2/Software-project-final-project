input_file = '/a/home/cc/students/cs/danielbarlev/Software-project-final-project/data/output_3.txt'
output_file = '/a/home/cc/students/cs/danielbarlev/Software-project-final-project/data/output_3.txt'

# Read input file and replace commas with spaces
with open(input_file, 'r') as infile:
    content = infile.read()
    content = content.replace(',', ' ')

# Write modified content to output file
with open(output_file, 'w') as outfile:
    outfile.write(content)

print(f"Commas replaced with spaces and saved to {output_file}")