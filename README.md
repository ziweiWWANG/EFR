# For academic use only

## A Linear Comb Filter for Event Flicker Removal
Event cameras are bio-inspired sensors that capture per-pixel asynchronous intensity change rather than the synchronous absolute intensity frames captured by a classical camera sensor.
Such cameras are ideal for robotics applications since they have high temporal resolution, high dynamic range and low latency.
However, due to their high temporal resolution, event cameras are particularly sensitive to flicker such as from fluorescent or LED lights.
During every cycle from bright to dark, pixels that image a flickering light source generate many events that provide little or no useful information for a robot, swamping the useful data in the scene.
In this paper, we propose a novel linear filter to preprocess event data to remove unwanted flicker events from an event stream.
The proposed algorithm achieves over 4.6 times relative improvement in the signal-to-noise ratio when compared to the raw event stream due to the effective removal of flicker from fluorescent lighting.
Thus, it is ideally suited to robotics applications that operate in indoor settings or scenes illuminated by flickering light sources.


Ziwei Wang, Dingran Yuan, Yonhon Ng, and Robert Mahony

The paper was accepted by the International Conf. Robotics and Automation (ICRA 2022) in Philadelphia, USA.

## Publications
arXiv preprint: https://arxiv.org/abs/2205.08090

## Video, Dataset and Code
Coming soon


If you have any questions or suggestions, please don't hesitate to get in touch with ziwei.wang1@anu.edu.au



