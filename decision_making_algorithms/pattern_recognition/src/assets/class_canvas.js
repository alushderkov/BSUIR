"use strict"

export class CanvasClass {
    Class;
    colour;

    constructor(Class, colour) {
        this.Class = Class;
        this.colour = colour;
    }

    drawClass(canvas) {

        function drawPoint(ctx, point, size) {
            ctx.beginPath();
            ctx.arc(point.x, point.y, size, 0, 2 * Math.PI);
            ctx.fill();
        }

        if ( canvas.getContext ) {
            const ctx = canvas.getContext("2d");
            ctx.fillStyle = this.colour;

            for (let point of this.Class.points) {
                drawPoint(ctx, point, 1);
            }

            drawPoint(ctx, this.Class.kernel, 4);

        }
    }
}