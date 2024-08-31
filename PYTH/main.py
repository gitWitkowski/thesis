import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from pandas.plotting import table

df = pd.read_csv('../data/data.txt', delimiter=';')
df_sorted = df.sort_values(by=['entropy'])
df_sorted.index = [''] * len(df)
rows, columns = df_sorted.shape

fig, ax = plt.subplots(figsize=(20, 10))
ax.axis('off') 
tbl = table(ax, df_sorted, loc='center', cellLoc='center', colWidths=[0.11]*len(df.columns))
tbl.auto_set_font_size(False)
tbl.set_fontsize(12)
tbl.scale(1.2, 1.2)

for (i, j), cell in tbl.get_celld().items():
    if i == 0:
        cell.set_text_props(weight='bold')
    cell.set_text_props(ha='center', va='center')
    cell.set_edgecolor('black')
    cell.set_linewidth(1)

plt.savefig('../data/data.png', bbox_inches='tight', dpi=300)
plt.close()

df['dist_comp'] = df['distribution'] + ' | ' + df['compression']

sns.set(style="darkgrid")

custom_colors = ['#ff2d00', '#941a00', '#00ffdb', '#00947f']

plt.figure(figsize=(12, 10))

ax = sns.barplot(data=df, x='rounding_type', y='entropy', hue='dist_comp', palette=custom_colors)

for p in ax.patches:
    height = p.get_height()
    ax.annotate(format(height, '.2f'),
                (0.01 + p.get_x() + p.get_width() / 2., height - 0.5),
                ha='center', va='bottom',
                xytext=(0, 5),
                textcoords='offset points',
                fontsize=13,
                rotation=90,
                color='white',fontweight='bold')

plt.title('Entropy value for different rounding methods, distributions and compression', fontsize=18, fontweight='bold')
plt.xlabel('Rounding type')
plt.ylabel('Entropy [bit]')
plt.legend(title='Distribution | Compression', bbox_to_anchor=(1, 0.85), loc='center right', fontsize=13)
plt.xticks(rotation=45)
plt.tight_layout()
plt.savefig('../data/entropy_values_comparison.png')  # Save the figure to a file (e.g., 'entropy_plot.png')