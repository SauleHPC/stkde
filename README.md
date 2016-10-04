`+-------------------------------------------------------------------------------------------------------------------------+-----+-----+----------+---------+
|                                                                                FLOP_COUNT_DIFFERENT_SCENARIOS[in GFLOPs]|NAIVE|IMPVD|REUSE_DISK|REUSE_BAR|
+-------------------------------------------------------------------------------------------------------------------------+-----+-----+----------+---------+
|   LOW_RESOLUTION, LOW_SPATIAL_BANDWIDTH, LOW_TEMPORAL_BANDWIDTH, POINTS_PER_BOX: 50, RATIO_BOX_VOLUME_TOTAL_VOLUME: 0.01| 2.70| 0.24|      0.14|     0.07|
|  LOW_RESOLUTION, LOW_SPATIAL_BANDWIDTH, HIGH_TEMPORAL_BANDWIDTH, POINTS_PER_BOX: 50, RATIO_BOX_VOLUME_TOTAL_VOLUME: 0.01| 3.09| 0.48|      0.28|     0.13|
|  LOW_RESOLUTION, HIGH_SPATIAL_BANDWIDTH, LOW_TEMPORAL_BANDWIDTH, POINTS_PER_BOX: 50, RATIO_BOX_VOLUME_TOTAL_VOLUME: 0.01|15.27| 6.00|      3.38|     1.80|
| LOW_RESOLUTION, HIGH_SPATIAL_BANDWIDTH, HIGH_TEMPORAL_BANDWIDTH, POINTS_PER_BOX: 50, RATIO_BOX_VOLUME_TOTAL_VOLUME: 0.01|23.77|12.01|      6.76|     3.27|
|  HIGH_RESOLUTION, LOW_SPATIAL_BANDWIDTH, LOW_TEMPORAL_BANDWIDTH, POINTS_PER_BOX: 50, RATIO_BOX_VOLUME_TOTAL_VOLUME: 0.01|10.79| 0.96|      0.55|     0.29|
| HIGH_RESOLUTION, LOW_SPATIAL_BANDWIDTH, HIGH_TEMPORAL_BANDWIDTH, POINTS_PER_BOX: 50, RATIO_BOX_VOLUME_TOTAL_VOLUME: 0.01|12.37| 1.92|      1.10|     0.52|
| HIGH_RESOLUTION, HIGH_SPATIAL_BANDWIDTH, LOW_TEMPORAL_BANDWIDTH, POINTS_PER_BOX: 50, RATIO_BOX_VOLUME_TOTAL_VOLUME: 0.01|61.11|24.02|     13.47|     7.19|
|HIGH_RESOLUTION, HIGH_SPATIAL_BANDWIDTH, HIGH_TEMPORAL_BANDWIDTH, POINTS_PER_BOX: 50, RATIO_BOX_VOLUME_TOTAL_VOLUME: 0.01|95.13|48.03|     26.93|    13.09|
+-------------------------------------------------------------------------------------------------------------------------+-----+-----+----------+---------+`