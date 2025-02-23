"use strict"

export class Point {
    _x_coordinate;
    _y_coordinate;

    constructor(x, y) {
        function random(min, max) {
            const result = min + Math.random() * (max - min);
            return Math.round(result);
        }

        this._x_coordinate = x ? x : random(1, 600);
        this._y_coordinate = y ? y : random(1, 300);

    }

    get x() { return this._x_coordinate; }
    get y() { return this._y_coordinate; }
}