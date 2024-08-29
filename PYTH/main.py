import pandas as pd
import matplotlib.pyplot as plt
from pandas.plotting import table

df = pd.read_csv('../data/data.txt', sep=';')
df = df.sort_values(by=['entropy'])
df.index = [''] * len(df)
rows, columns = df.shape

fig, ax = plt.subplots(figsize=(20, 10))
ax.axis('off') 
tbl = table(ax, df, loc='center', cellLoc='center', colWidths=[0.11]*len(df.columns))
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

exp_df = df[df['distribution'] == 'exponential']

exp_df_compressed = exp_df[exp_df['compression'] == 'Z_BEST_COMPRESSION']

exp_df_not_compressed = exp_df[exp_df['compression'] == 'NO']


uni_df = df[df['distribution'] == 'uniform']

uni_df_compressed = uni_df[uni_df['compression'] == 'Z_BEST_COMPRESSION']

uni_df_not_compressed = uni_df[uni_df['compression'] == 'NO']


ax = uni_df_compressed.plot.bar(x='rounding_type', y='entropy', legend=True, color='skyblue')
ax.set_xlabel('rounding type')
ax.set_ylabel('entropy')
ax.set_title('Entropy for uniform distribution with compression')
ax.grid(True)
plt.savefig('../data/uni_compression.png', bbox_inches='tight', dpi=300)
plt.close()

ax = uni_df_not_compressed.plot.bar(x='rounding_type', y='entropy', legend=True, color='skyblue')
ax.set_xlabel('rounding type')
ax.set_ylabel('entropy')
ax.set_title('Entropy for uniform distribution without compression')
ax.grid(True)
plt.savefig('../data/uni_no_compression.png', bbox_inches='tight', dpi=300)
plt.close()

ax = exp_df_compressed.plot.bar(x='rounding_type', y='entropy', legend=True, color='skyblue')
ax.set_xlabel('rounding type')
ax.set_ylabel('entropy')
ax.set_title('Entropy for exponential distribution with compression')
ax.grid(True)
plt.savefig('../data/exp_compression.png', bbox_inches='tight', dpi=300)
plt.close()

ax = uni_df_not_compressed.plot.bar(x='rounding_type', y='entropy', legend=True, color='skyblue')
ax.set_xlabel('rounding type')
ax.set_ylabel('entropy')
ax.set_title('Entropy for exponential distribution without compression')
ax.grid(True)
plt.savefig('../data/exp_no_compression.png', bbox_inches='tight', dpi=300) 
plt.close()