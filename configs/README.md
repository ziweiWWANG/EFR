# A Linear Comb Filter for Event Flicker Removal

#### Main parameters in configuration file `EFR_config.yaml`

|          Variables            | Description | Default Value |
|----------------------|----------------------|-----------------------------|
| `base_frequency`    | Frequency of the flicker in Hz | 50 |
| `process_ts_start`   | Start time to process the event data in second | 0 |
| `process_ts_end`     | End time to process the event data in second   | 2.5 |
| `rho1`     |  Tuning parameter in Eq. (7). A higher value localises the filter effect close to the notch frequency and ensures that other frequencies are not distorted, while a lower value widens both the notch and the associated frequency distortion up to the limit 0  | 0.6 |
| `delta_t`     | Parameter to control the time length of the circular queue (unit microsecond). If the timestamp of the coming new event is close to the first event in the circular queue (the gap is less than `delta_t`), update the event value and timestamp of the first value in the queue and don't push a new event to the queue. A larger value leads to fast filtering speed   | 10000 |
| `sampler_threshold`     | Event threshold value of the event sampler. Trigger and save events once the per-pixel accumulated intensity is higher than the threshold. Using a bigger value means more events will be combined, leading to fewer generated events | 0.7 |
| `load_or_compute_bias` | True: load pre-calibrated event bias file from "../data/parameters/bias.csv"; False: use the input event data to estimate event bias. The bias is computed following the event calibration algorithm - Event Camera Calibration of Per-pixel Biased Contrast Threshold [[Paper]](https://ssl.linklings.net/conferences/acra/acra2019_proceedings/views/includes/files/pap135s1-file1.pdf)[[arXiv]](https://arxiv.org/pdf/2012.09378.pdf), ACRA 2019 | 1 |
| `img_height`     | Maximum height of input event data | 480 |
| `img_width`      | Maximum width of input event data | 640 |
| `time_resolution`    | `1 / time_resolution` (second) is the minimum time resolution used. Using a higher value may increase the performance but takes longer computational time  | 1000000 |
| `input_event`    | File name of the input raw event camera data | "log.raw" |
| `output_event`   | File name of the output filtered event camera data | "events_filter.txt" |
| `data_id`        | Input event camera dataset name | "02" |
