"use strict"

import {Class} from "./class.js";
import {Point} from "./point_class.js";
import {calcDist} from "./functions.js";

export class K_mid_algorithm {
    _points;
    _kernels;
    _classes;
    constructor(classes) {
        this._points = [];
        this._kernels = [];
        this._classes = classes ? classes : [];
    }

    createPoints(amount) {

        for (let i = 0; i < amount; i++) {
            this._points[i] = new Point();
        }
    }

    definePoints() {

        for (let elem of this._classes) {
            this._kernels.push(elem.kernel);
            this._points = [...this._points, ...elem.points];
        }
    }

    createClasses(classes_amount, points_amount) {

        for (let i = 0; i < classes_amount; i++) {
            this._classes[i] = new Class();
            this._kernels.push( this._classes[i].kernel );
        }

        this.createPoints(points_amount);
    }

    classForPoint(point) {
        let result = this._classes[0];
        let minDist = calcDist(point, result.kernel);

        for (let elem of this._classes) {
            let currentDist = calcDist(point, elem.kernel);

            if (minDist > currentDist) {
                minDist = currentDist;
                result = elem;
            }
        }

        return result;
    }

    clearClasses() {

        for (let elem of this._classes) {
            elem.points = [];
        }
    }

    relatePointsToClasses() {

        this.clearClasses();

        for (let elem of this._points) {
            this.classForPoint(elem).points.push(elem);
        }
    }

    oldKernelPosition(aClass) {
        return this._kernels.indexOf(aClass.kernel);
    }

    defineNewKernels() {

        for (let elem of this._classes) {
            this._kernels[ this.oldKernelPosition(elem) ] = elem.calcNewKernel();
        }
    }

    isArraysEqual(firstArray, secondArray) {
        return firstArray.toString() === secondArray.toString();
    }
    isEnd() {
        let result;
        let oldKernels = [];

        Object.assign(oldKernels, this._kernels);
        this.defineNewKernels();
        result = this.isArraysEqual(oldKernels, this._kernels);

        return result;
    }

    algorithmResult() {
        do {
            this.relatePointsToClasses();
        } while ( !this.isEnd() )
    }

}