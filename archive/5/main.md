# ケロシゼロ Model [5]

from `4.pt` , use `buffer/1` too.

add height reward weight when attack is very low:

```c++
std :: vector<bool> tag(batch);
for (int I = 0; I < batch; I ++) {
    auto & t = tetris[I];
    tag[I] = t.pieces >= 17 && t.attack <= 5;
}

float get_ex_V(const Tetris & t, bool o = false) {
	if (! o) return 0;

	int top = 30;
	for (int y = 29; y >= 0; y --) {
		if (t.b[y]) break;
		top = y;
	}
	int height = 20 - top;
	return height * 0.7f;
}
```

and change the sample collector:

```c++
std :: vector<TetrisTrainData> get_samples(int sample, TetrisBuffer & buffer) {
	std :: vector<TetrisTrainData> samples;
	if (buffer.tot == 0) return samples;

	auto V_max_data = buffer.sample_by_sort_id(buffer.tot - 1);
	float V_max = V_max_data.V;

	for (int _ = 0; _ < sample * 2; _ ++) {
		auto data = buffer.sample_recent();
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
bin/trainer model/4.pt model/4.opt buffer/1 1000 500 128 256
```

a sample game:

```text
LIJOZTSJZLTOSIZJSLITOZTJLSOILOSTZIJSZJITOL
DLsDAsCLslsCRZsCrrsDLssDLsRlDAsLrsusuRsrrsuLsRDZsCLDusDAsCZRslsCrrsRDZsuLsDRAsDAsCllDusCLsRlssRlDAsrrDAsCZRsCllDusCZRsDRZs
```
