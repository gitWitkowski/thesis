import matplotlib.pyplot as plt
import matplotlib.patheffects as path_effects
import seaborn as sns
import re
import os
import math

DATA_DIR_PATH = "/home/witkowski/repos/thesis/data/bit_entropy" 
IMG_DIR_PATH = "/home/witkowski/repos/thesis/data/img/bit_entropy"

def parse_entropy(text):
    pattern = r"bit\s+(\d+):\s+([\d\.]+)"
    matches = re.findall(pattern, text)
    data_dict = {int(m[0]): float(m[1]) for m in matches}
    return [data_dict.get(i, 0.0) for i in range(32)]

for file_name in os.listdir(DATA_DIR_PATH):
    with open(os.path.join(DATA_DIR_PATH, file_name), 'r', encoding='utf-8') as file:
        data_raw_text = file.read()

    entropy_values = parse_entropy(data_raw_text)
    bits = list(range(32))

    sns.set_theme(style="whitegrid")
    plt.figure(figsize=(15, 7))

    colors = ['#4C72B0'] * 23 + ['#55A868'] * 8 + ['#C44E52'] * 1

    bars = plt.bar(bits, entropy_values, color=colors, edgecolor='black', alpha=0.8)

    for bar in bars:
        height = bar.get_height()
    
        if height > 0.2:
            label_y = height / 2
        else:
            label_y = 0.2

        text_color = 'white'
        outline_color = 'black'

        txt = plt.text(
            bar.get_x() + bar.get_width() / 2, 
            label_y, 
            f'{height:.7f}', 
            ha='center', 
            va='center', 
            rotation=90, 
            fontsize=10, 
            fontweight='bold', 
            color=text_color
        )

        txt.set_path_effects([
            path_effects.withStroke(linewidth=1, foreground=outline_color)
        ])

    plt.axhline(y=1.0, color='#e74c3c', linestyle='--', lw=2, label='Max entropy (1.0 bit)')

    plt.gca().invert_xaxis()

    plt.title(f'Bit entropy for float values {file_name.split(".")[0]}', fontsize=16, pad=20)
    plt.ylabel('Entropy [bit]', fontsize=12)
    plt.xticks(bits)
    plt.ylim(0, 1.1)

    plt.text(31, -0.12, 'Sign', color='#C44E52', ha='center', fontweight='bold')
    plt.text(26.5, -0.12, 'Exponent', color='#55A868', ha='center', fontweight='bold')
    plt.text(11, -0.12, 'Mantissa', color='#4C72B0', ha='center', fontweight='bold')

    plt.legend(loc='upper right')
    plt.tight_layout()
    plt.savefig(os.path.join(IMG_DIR_PATH, f'bit_entropy_bar_plot_{file_name.split(".")[0]}.png'), dpi=300)

    plt.figure(figsize=(12, 6))

    plt.plot(bits, [(1.0-x) for x in entropy_values], label=r'$1-entropy$', color='b', marker='o', linestyle='-', linewidth=2, markersize=6)
    plt.axvline(x=22.5, color='red', linestyle=':', linewidth=2)
    plt.axvline(x=30.5, color='red', linestyle=':', linewidth=2)
    plt.text(31, -0.12, 'Sign', color='#C44E52', ha='center', fontweight='bold')
    plt.text(26.5, -0.12, 'Exponent', color='#55A868', ha='center', fontweight='bold')
    plt.text(11, -0.12, 'Mantissa', color='#4C72B0', ha='center', fontweight='bold')
    plt.plot(bits, [(1.0-x)**2 for x in entropy_values], label=r'${(1-entropy)}^2$', color='orange', marker='o', linestyle='-', linewidth=2, markersize=6)
    plt.title(f'Bit entropy {file_name.split(".")[0]}', fontsize=16)
    plt.xlabel('Bit number', fontsize=12)
    plt.ylabel('Entropy [bit]', fontsize=12)

    plt.xticks(bits)

    plt.grid(True, which='both', linestyle='--', alpha=0.7)

    plt.ylim(0, 1.05)

    plt.legend()

    plt.tight_layout()
    plt.savefig(os.path.join(IMG_DIR_PATH, f'entropy_func_{file_name.split(".")[0]}.png'), dpi=300)
    plt.close()


plt.figure(figsize=(12, 6))

with open('/home/witkowski/repos/thesis/data/bit_entropy/trk_pt_all_poly_1.txt', 'r', encoding='utf-8') as file:
        data_raw_text_poly_1 = file.read()
with open('/home/witkowski/repos/thesis/data/bit_entropy/trk_pt_all_poly_2.txt', 'r', encoding='utf-8') as file:
        data_raw_text_poly_2 = file.read()
with open('/home/witkowski/repos/thesis/data/bit_entropy/trk_pt_all_poly_3.txt', 'r', encoding='utf-8') as file:
        data_raw_text_poly_3 = file.read()
with open('/home/witkowski/repos/thesis/data/bit_entropy/trk_pt_all.txt', 'r', encoding='utf-8') as file:
        data_raw_text = file.read()


bits = list(range(32))
entropy_values = parse_entropy(data_raw_text)
entropy_values_poly_1 = parse_entropy(data_raw_text_poly_1)
entropy_values_poly_2 = parse_entropy(data_raw_text_poly_2)
entropy_values_poly_3 = parse_entropy(data_raw_text_poly_3)

plt.plot(bits, [(1.0-x) for x in entropy_values], label=r'original data', marker='o', linestyle='-', linewidth=2, markersize=6)
plt.plot(bits, [(1.0-x) for x in entropy_values_poly_1], label=r'$f_1(x)=\pi x^4 + \sqrt{2} x^3 + e x^2 + 1.234567x$', marker='o', linestyle='-', linewidth=2, markersize=6)
plt.plot(bits, [(1.0-x) for x in entropy_values_poly_2], label=r'$f_2(x)=\sin(2^{15} x)$', marker='o', linestyle='-', linewidth=2, markersize=6)
plt.plot(bits, [(1.0-x) for x in entropy_values_poly_3], label=r'$f_3(x) = \ln(x)$', marker='o', linestyle='-', linewidth=2, markersize=6)
plt.axvline(x=22.5, color='red', linestyle=':', linewidth=2)
plt.axvline(x=30.5, color='red', linestyle=':', linewidth=2)
plt.text(31, -0.12, 'Sign', color='#C44E52', ha='center', fontweight='bold')
plt.text(26.5, -0.12, 'Exponent', color='#55A868', ha='center', fontweight='bold')
plt.text(11, -0.12, 'Mantissa', color='#4C72B0', ha='center', fontweight='bold')
plt.title(f'Bit entropy comparison', fontsize=16)
plt.xlabel('Bit number', fontsize=12)
plt.ylabel(r'$1-H$', fontsize=12)
plt.xticks(bits)
plt.grid(True, which='both', linestyle='--', alpha=0.7)
plt.ylim(0, 1.05)
plt.legend()
plt.tight_layout()
plt.savefig(os.path.join(IMG_DIR_PATH, f'entropy_func_comparison.png'), dpi=300)
plt.close()