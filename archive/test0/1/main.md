# ケロシゼロ Model [1]

from `0.pt` , use `buffer/1`.

task: place 35 piece to maximum attack.

use the height of top reward:

```c++
float get_ex_V(const Tetris & t) {
	int top = 30;
	for (int y = 29; y >= 0; y --) {
		if (t.b[y]) break;
		top = y;
	}
	return (20 - top) * 0.7f;
}
```

sample collector:

```c++
std :: vector<TetrisTrainData> get_samples(int sample, TetrisBuffer & buffer) {
	std :: vector<TetrisTrainData> samples;
	if (buffer.tot == 0) return samples;

	auto V_max_data = buffer.sample_by_sort_id(buffer.tot - 1);
	float V_max = V_max_data.V;

	for (int _ = 0; _ < sample; _ ++) {
		auto data = buffer.sample_recent();
		samples.push_back(data);
	}

	for (int _ = 0; _ < sample; _ ++) {
		auto data = buffer.sample_high_V();
		samples.push_back(data);
	}

	for (auto & data : samples) {
		random_mask(data);
		float V = data.V;
		data.PW = std :: powf((V + 1) / (V_max + 1), 2);
	}

	return samples;
}
```

train command:

```shell
bin/trainer model/0.pt empty buffer/1 1000 200 128 256
```

and get `1.pt`, `1.opt` and `1.log`.

a sample game:

```text
ZOLJITSLJZOSTITLJISZOOJIZLSTIZLJSOTTSJZIOL
DLsDRlsCrDZsCDAsllDAsrDAsLDAsCDLsCuDAsCRDusCrDsClDsCRDusCDLusrDusCRDAsCrruDsCllDAsllZDsCRlDuuslDsDAsCrrDsLDZsCLDZsCZRDsDLusuRDslDsZRDsrDusRDusCRuDsCLDslDus
```
