# ケロシゼロ Model [6]

from `5.pt` , use `buffer/1` too.

height reward weight same as `5.pt`

the Policy Loss changed to:

```c++
data.PW = std :: powf((V + 1) / (V_max + 1), 1);
```

train command:

```shell
bin/trainer model/5.pt model/5.opt buffer/1 3000 500 128 256
```
