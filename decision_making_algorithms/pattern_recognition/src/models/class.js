"use strict"

import {Point} from "./point_class.js";
import {arithmeticAverage} from "./functions.js";

export class Class {
    points;
    _kernel;

    constructor(kernel) {
        this.points = [];
        this._kernel = kernel ? kernel : new Point();
    }

    get kernel() { return this._kernel; }
    set kernel(aPoint) { this._kernel = aPoint; }

    calcNewKernel() {
        let result;
        const newKernel = arithmeticAverage(this.points);

        this._kernel = new Point(newKernel.x, newKernel.y);
        result = this._kernel;

        return result;
    }

}