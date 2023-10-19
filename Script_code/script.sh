#!/bin/bash

# Run the 'sensors' command and store the output in a variable
sensors_output="$(sensors)"

# Define the output file path
output_file="sensors_output.txt"

# Save the output to the specified file
echo "$sensors_output" > "$output_file"

# Display a message indicating the location of the output file
echo "Sensors data saved to $output_file"
