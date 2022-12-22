import pandas as pd
import os
import zipfile
import yaml
from yaml.loader import SafeLoader

# Open the file and load the file
with open('../configs/EFR_config.yaml') as f:
    data = yaml.load(f, Loader=SafeLoader)
input_name = data['output_event']
input_paths = os.path.join("../data/", data['data_id'], input_name)
iterator = pd.read_csv(input_paths,
                       delimiter=' ',
                       header=None,
                       names=['ts', 'y', 'x', 'p'],
                       dtype={
                           'ts': int,
                           'y': int,
                           'x': int,
                           'p': int
                       },
                       engine='c',
                       skiprows=1,
                       nrows=None,
                       memory_map=True)

sorted_e = iterator.sort_values(by=['ts'])

# Get col and row from input data
with open(input_paths) as f:
    col_row = f.readline()

# save filtered events
output_name = "events_filter_sort.txt"
output_paths = os.path.join("../data/", data['data_id'], output_name)
with open(output_paths, 'w') as f:
    f.write(col_row)
sorted_e.to_csv(output_paths, header=None, index=None, sep=' ', mode='a')
print("Sorted events saved to" + output_paths)

# zip filtered event data
zip_name = "events_filter_sort.zip"
zip_paths = os.path.join("../data/", data['data_id'], zip_name)
zipfile.ZipFile(zip_paths, mode='w').write(output_paths)
print("Zipped events saved to " + zip_paths)