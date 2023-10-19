#!/bin/bash

while true; do
    # Run the 'sensors' command and store the output in a variable
    sensors_output="$(sensors)"

    # Define the output file path
    output_file="sensors_output.txt"

    # Save the output to the specified file
    echo "$sensors_output" > "$output_file"

    # Display a message indicating the location of the output file
    echo "Sensors data saved to $output_file"

    # Ask the user if they want to continue or stop
    read -p "Do you want to run again? (yes/no): " answer

    # Check if the user wants to stop
    if [ "$answer" = "no" ]; then
        break
    fi
done
