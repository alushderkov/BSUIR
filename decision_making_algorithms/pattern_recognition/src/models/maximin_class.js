"use strict"

import {Class} from "./class.js";
import {Point} from "./point_class.js";
import {calcDist} from "./functions.js";

export class Maximin_algorithm {
    _points;
    _kernels;
    _classes;
    _steps;

    constructor() {
        this._points = [];
        this._kernels = [];
        this._classes = [];
        this._steps = [];
    }

    createPoints(amount) {

        for (let i = 0; i < amount; i++) {
            this._points[i] = new Point();
        }
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

    thresholdDistance() {
        let result;
        const length = this._kernels.length;
        let sum = 0;
        let amount = 0;

        for (let i = 0; i < length - 1; i++) {
            let current_kernel = this._kernels[i];

            for (let j = i + 1; j < length; j++) {
                let another_kernel = this._kernels[j];

                sum += calcDist(current_kernel, another_kernel);
                amount++;
            }
        }

        if (amount) {
            result = Math.round( sum / (amount * 2) );
        } else {
            result = -1;
        }

        return result;
    }

    findMaxRemotePoint(Class) {
        let result = Class.points[0];
        let max_dist = calcDist(result, Class.kernel);

        for (let elem of Class.points) {
            let currentDist = calcDist(elem, Class.kernel);

            if (currentDist > max_dist) {
                currentDist = max_dist;
                result = elem;
            }
        }

        return result;
    }

    isNewKernel(Class) {
        return calcDist( this.findMaxRemotePoint(Class), Class.kernel ) > this.thresholdDistance();
    }

    addNewStep() {
        const deepCopyClasses = this._classes.map(cls => {

            const newClass = new Class(cls.kernel);

            newClass.points = cls.points.map(point => new Point(point.x, point.y));

            return newClass;
        });

        this._steps.push(deepCopyClasses);
    }

    createNewClass() {
        let result = false;

        for (let elem of this._classes) {

            if( this.isNewKernel(elem) ) {
                const newKernel = this.findMaxRemotePoint(elem);
                const newClass = new Class(newKernel);

                this._classes.push(newClass);
                this._kernels.push(newClass.kernel);

                this.relatePointsToClasses();

                this.addNewStep();

                result = true;
            }

        }

        return result;
    }

    startAlgorithm(points_amount) {
        this.createPoints(points_amount);

        const firstClass = new Class();

        this._classes[0] = firstClass;
        this._kernels.push(firstClass.kernel);

        this.addNewStep();

        this.relatePointsToClasses();
    }
    algorithmResult(points_amount) {
        this.startAlgorithm(points_amount);
        do {} while ( this.createNewClass() );
    }
}
