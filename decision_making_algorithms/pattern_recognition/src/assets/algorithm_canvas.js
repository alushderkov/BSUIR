"use strict"

import {CanvasClass} from "./class_canvas.js";

export class AlgorithmCanvas {
    classes;

    constructor(classes) {
        this.classes = classes;
    }

    drawAlgorithm(canvas) {

        function getRandomColor() {
            let letters = '0123456789ABCDEF';
            let result = '#';
            for (let i = 0; i < 6; i++) {
                result += letters[Math.floor(Math.random() * 16)];
            }
            return result;
        }

        for (let elem of this.classes) {
            let canvas_class = new CanvasClass(elem, getRandomColor() );
            canvas_class.drawClass(canvas);
        }
    }

}