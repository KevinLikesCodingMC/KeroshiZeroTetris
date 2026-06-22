// ==UserScript==
// @name         Keroshi Tetrio Socket
// @namespace    http://tampermonkey.net/
// @version      2026-06-22
// @description  Keroshi Tetrio Socket
// @author       Keroshi
// @match        http://127.0.0.1:8000/*
// @grant        GM_xmlhttpRequest
// ==/UserScript==

(function() {
	'use strict';

	let lst_id = - 1;

	setInterval(async () => {
		GM_xmlhttpRequest({
			method: "GET",
			url:
				"http://127.0.0.1:8014/tetrio_f_socket.txt?" +
				Date.now(),

			onload: function(resp) {
				const file = resp.responseText;

				if (! file) {
					lst_id = - 1;
					return;
				}

				const [id_str, seq] = file.trim().split(/\s+/);
				const id = Number(id_str);

				if (id != lst_id && id != - 1) {
					lst_id = id;

					let bag = Array.from(seq.toLowerCase());
					unsafeWindow.keroshi_S.falling.type = bag[0];
					bag.shift();
					unsafeWindow.keroshi_S.bag = bag;
				}
			},

			onerror: function(err) {
				console.error("ERROR");
				console.error(err);
			}
		});
	}, 100);

})();