import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
from pandas.plotting import table

# read data from file
df = pd.read_csv('../data/data.txt', delimiter=';')
# sort by entropy value
df_sorted = df.sort_values(by=['entropy'])
# erase row names
df_sorted.index = [''] * len(df)
rows, columns = df_sorted.shape

# plot for entropy values table
fig, ax = plt.subplots(figsize=(20, 10))
ax.axis('off') 
t = table(ax, df_sorted, loc='center', cellLoc='center', colWidths=[0.11]*len(df.columns))
t.auto_set_font_size(False)
t.set_fontsize(12)
t.scale(1.2, 1.2)

# format table
for (i, j), cell in t.get_celld().items():
    if i == 0:
        # bold header
        cell.set_text_props(weight='bold') 
    # center text in a cell

# save table
plt.savefig('../data/img/data.pdf', bbox_inches='tight', dpi=300)
plt.close()


# new column with both distribution and compression
df['dist_comp'] = df['distribution'] + ' | ' + df['compression']

# plot formatting
sns.set(style="darkgrid")
# plot bar colors
custom_colors = ['#ff2d00', '#941a00', '#00ffdb', '#00947f']
# plot size, ratio
plt.figure(figsize=(12, 10))

# create bar plot
ax = sns.barplot(data=df, x='rounding_type', y='entropy', hue='dist_comp', palette=custom_colors)

# add entropy value for every bar
for p in ax.patches:
    ax.annotate(format(p.get_height(), '.2f'), # text
                (0.01 + p.get_x() + p.get_width() / 2., p.get_height() - 0.5), # annotate position
                ha='center', va='bottom', # alignment
                xytext=(0, 5), # text pos
                textcoords='offset points', # xytext coordinate system
                fontsize=13,
                rotation=90, # vertical text
                color='white',fontweight='bold')

# plot properties
plt.title('Entropy value for different rounding methods, distributions and compression', fontsize=18, fontweight='bold')
plt.xlabel('Rounding type')
plt.ylabel('Entropy [bit]')
plt.legend(title='Distribution | Compression', bbox_to_anchor=(1, 0.85), loc='center right', fontsize=13)
plt.xticks(rotation=45) # rotated x axis labels
plt.tight_layout() # adjust the padding

# save plot
plt.savefig('../data/img/entropy_values_comparison.pdf')


# read data from file
df = pd.read_csv('../data/vector_data_PxPyPz.txt', delimiter=';')

# unpivot df
df = df.melt(id_vars="round_indicator", var_name="var", value_name="values")

# plot size
fig, ax = plt.subplots(figsize=(8,8))

# draw data as barplot
sns.barplot(data=df, x="round_indicator", y="values", hue="var")

# plot title and axis labels
plt.title("Shannon entropy and compression factor after\nconversion to PxPyPz, based on rounding operation", fontsize=20)
plt.ylabel("Metric value")
plt.xlabel("Applied transformation")

# add legend and custom entries names
handles, _ = ax.get_legend_handles_labels()
plt.legend(handles=handles, labels=['Entropy [bit]', 'Compression factor'], title='Metric type', loc='center right')

# add value for every bar
for p in ax.patches:
    ax.annotate(format(p.get_height(), '.2f'), # text
                (0.01 + p.get_x() + p.get_width() / 2., p.get_height() - 0.5), # annotate position
                ha='center', va='bottom', # alignment
                xytext=(0, 5), # text pos
                textcoords='offset points', # xytext coordinate system
                fontsize=13,
                color='white',fontweight='bold')

# save plot as image
plt.savefig('../data/img/vector_PxPyPz.pdf')


# read data from file
df = pd.read_csv('../data/vector_data_PtEtaPhi.txt', delimiter=';')

# unpivot df
df = df.melt(id_vars="round_indicator", var_name="var", value_name="values")

# plot size
fig, ax = plt.subplots(figsize=(8,8))

# draw data as barplot
sns.barplot(data=df, x="round_indicator", y="values", hue="var")

# plot title and axis labels
plt.title("Shannon entropy and compression factor for PtEtaPhi,\nbased on rounding operation", fontsize=20)
plt.ylabel("Metric value")
plt.xlabel("Applied transformation")

# add legend and custom entries names
handles, _ = ax.get_legend_handles_labels()
plt.legend(handles=handles, labels=['Entropy [bit]', 'Compression factor'], title='Metric type')

# add value for every bar
for p in ax.patches:
    ax.annotate(format(p.get_height(), '.2f'), # text
                (0.01 + p.get_x() + p.get_width() / 2., p.get_height() - 0.5), # annotate position
                ha='center', va='bottom', # alignment
                xytext=(0, 5), # text pos
                textcoords='offset points', # xytext coordinate system
                fontsize=13,
                color='white',fontweight='bold')

# save plot as image
plt.savefig('../data/img/vector_PtEtaPhi.pdf')
