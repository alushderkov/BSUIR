"use strict"

import {AlgorithmCanvas} from "./algorithm_canvas.js";
import {K_mid_algorithm} from "../models/k_mid_class.js";

const canvas = document.getElementsByClassName("algorithm")[0];

const K_MID = new K_mid_algorithm();
K_MID.createClasses(5, 100000);
K_MID.algorithmResult();

const K_MID_CANVAS = new AlgorithmCanvas(K_MID);
K_MID_CANVAS.drawAlgorithm(canvas);
