"use strict"

import {AlgorithmCanvas} from "./algorithm_canvas.js";
import {K_mid_algorithm} from "../models/k_mid_class.js";
import {Maximin_algorithm} from "../models/maximin_class.js";

const canvas = document.getElementsByClassName("algorithm")[0];

const MAXIMIN = new Maximin_algorithm();
MAXIMIN.algorithmResult(100000);

let step = 0;
canvas.addEventListener("click", () => {
    let maximin_canvas;

    if (step < MAXIMIN._steps.length) {
        maximin_canvas = new AlgorithmCanvas(MAXIMIN._steps[step]);
        maximin_canvas.drawAlgorithm(canvas);
        step++;
    } else {
        const K_MID_MAXIMIN = new K_mid_algorithm(MAXIMIN._classes);
        K_MID_MAXIMIN.definePoints();
        K_MID_MAXIMIN.algorithmResult();

        let k_mid_maximin_canvas = new AlgorithmCanvas(K_MID_MAXIMIN._classes);
        k_mid_maximin_canvas.drawAlgorithm(canvas);
    }
})

