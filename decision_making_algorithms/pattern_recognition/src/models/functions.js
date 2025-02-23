"use strict"
export function calcDist(point1, point2) {
    let result_x = Math.pow(point2.x - point1.x, 2);
    let result_y = Math.pow(point2.y - point1.y, 2);
    return Math.sqrt(result_x + result_y);
}

export function arithmeticAverage(points) {
    let result = {};
    let sum_x = 0;
    let sum_y = 0;
    let points_amount = 0;

    for (let elem of points) {
        sum_x += elem.x;
        sum_y += elem.y;
        points_amount++;
    }

    result.x = Math.round(sum_x / points_amount);
    result.y = Math.round(sum_y / points_amount);

    return result;
}
