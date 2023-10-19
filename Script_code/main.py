import psutil
import subprocess
import re

def get_cpu_temperature():
    try:
        # Use o comando 'sensors' para obter a temperatura da CPU em sistemas Linux
        output = subprocess.check_output(['sensors'], universal_newlines=True)
        temperature_lines = re.findall(r'\S+\s+\d+\.\d+\s°C', output)
        if temperature_lines:
            cpu_temperature = temperature_lines[0].split()[-1]
            return f'CPU Temperature: {cpu_temperature}'
    except Exception as e:
        return f'Error reading CPU temperature: {e}'

def get_gpu_temperature():
    # Simplesmente retorne uma mensagem indicando que não há suporte para a GPU
    return 'GPU Temperature: Not supported (No NVIDIA GPU found)'

if __name__ == '__main__':
    cpu_temperature = get_cpu_temperature()

    print(cpu_temperature)
