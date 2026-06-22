function KeroshiSetSeq(seq_str) {
	seq = Array.from(seq_str.toLowerCase().trim());
	window.keroshi_S.falling.type = seq[0];
	seq.shift();
	window.keroshi_S.bag = seq;
}
