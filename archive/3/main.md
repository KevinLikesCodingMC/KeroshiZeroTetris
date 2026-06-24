# ケロシゼロ Model [3]

from `2.pt` , use `buffer/1` too.

lower height reward weight, from $0.5$ to $0.2$.

more mcts search steps.

train command:

```shell
bin/trainer model/2.pt model/2.opt buffer/1 1000 500 128 256
```

a sample game:

```text
JIZTOLSIZLJOTSLZJITSOTOJSLZIOJLISTZZTIJOSL
DLZsCLZsLDuslsrusRsCDuslDAsRDAslDAsCDusCRlsLDusRlsrrDAslsZRsLDZsrrDuslsCLDusRsCRsCsllZsCDRsCrrDusCLsCsCDLsCZRsLDZsDAsrDZsZs
```
