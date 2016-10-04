hs_low, hs_high = 250, 2500
ht_low, ht_high = 1, 14
res_low, res_high = [100, 100, 1], [50, 50, 1]
ppb_variations = [50, 500, 2000, 5000]
ratio_variations = [0.01, 0.1, 0.3, 0.5]
runtime_variants = ["python", "pypy"]
program_variants = ["stkde_naive_wo_numpy.py"]

for runtime in runtime_variants:
	for program in program_variants:
		for hs in (hs_low, hs_high):
			for ht in (ht_low, ht_high):
				for res in (res_low, res_high):
					for ppb in ppb_variations:
						for ratio in ratio_variations:
							print runtime, program, hs, ht, res[0], res[1], res[2], ppb, ratio
							