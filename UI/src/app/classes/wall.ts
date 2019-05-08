import { Coordinate } from './coordinate';
import { Path, Point, PointText, CurveLocation } from 'paper';

export class Wall {
    path: Path;
    coordinates: Array<Coordinate> = [];
    direction: Direction;
    center: Coordinate;

    constructor(origin: Coordinate, direction: Direction, distances: number[]) {

        this.direction = direction;

        // Displacement values are added to measure the distance of the sensor to the origin
        // The vertical displacements are flipped to represent the grid starting at (0, 0) in the top left corner
        const displacementMeasurement = 5;

        let angle = 0;
        let leftSensorDisplacementX = 0;
        let leftSensorDisplacementY = 0;
        let rightSensorDisplacementX = 0;
        let rightSensorDisplacementY = 0;

        switch (direction) {
            case Direction.Forward:
                leftSensorDisplacementX = -displacementMeasurement;
                rightSensorDisplacementX = displacementMeasurement;
                break;
            case Direction.Right:
                angle = 90;
                leftSensorDisplacementY = -displacementMeasurement;
                rightSensorDisplacementY = displacementMeasurement;
                break;
            case Direction.Backward:
                angle = 180;
                leftSensorDisplacementX = displacementMeasurement;
                rightSensorDisplacementX = -displacementMeasurement;
                break;
            case Direction.Left:
                angle = 270;
                leftSensorDisplacementY = displacementMeasurement;
                rightSensorDisplacementY = -displacementMeasurement;
                break;
        }

        const leftSensorOrigin = new Coordinate(origin.x + leftSensorDisplacementX, origin.y + leftSensorDisplacementY);
        const rightSensorOrigin = new Coordinate(origin.x + rightSensorDisplacementX, origin.y + rightSensorDisplacementY);

        this.coordinates.push(this.calculateCoordinate(leftSensorOrigin, angle, distances[0]));
        this.coordinates.push(this.calculateCoordinate(rightSensorOrigin, angle, distances[1]));
        this.center = new Coordinate(
            Math.round((this.coordinates[0].x + this.coordinates[1].x) / 2),
            Math.round((this.coordinates[0].y + this.coordinates[1].y) / 2)
        );
    }

    calculateCoordinate(origin: Coordinate, angle: number, distance: number): Coordinate {
        // Convert angle degrees to radians
        const radians = angle * (Math.PI / 180);

        return new Coordinate(
            (origin.x + distance * Math.sin(radians)),
            (origin.y - distance * Math.cos(radians))
        );
    }

    draw() {
        if (this.path != null) {
            this.path.remove();
        }

        // Path setup
        this.path = new Path();
        this.path.strokeColor = 'black';

        // Create Points from Coordinates
        const start = new Point(this.coordinates[0].x, this.coordinates[0].y);
        const end = new Point(this.coordinates[1].x, this.coordinates[1].y);

        // Draw line
        this.path.moveTo(start);
        this.path.lineTo(end);
    }

    extend(extension: number) {

        if (this.direction === Direction.Forward || this.direction === Direction.Backward) {
            // +-1 x to both points
            if (this.coordinates[0].x > this.coordinates[1].x) {
                this.coordinates[0].x += extension;
                this.coordinates[1].x -= extension;
            } else {
                this.coordinates[0].x -= extension;
                this.coordinates[1].x += extension;
            }
        } else {
            // +-1 y to both points
            if (this.coordinates[0].y > this.coordinates[1].y) {
                this.coordinates[0].y += extension;
                this.coordinates[1].y -= extension;
            } else {
                this.coordinates[0].y -= extension;
                this.coordinates[1].y += extension;
            }
        }

        this.draw();
    }

    getLength() {
        if (this.direction === Direction.Forward || this.direction === Direction.Backward) {
            this.placeText('5m', this.path);
        } else {
            this.placeText('7m', this.path);
        }
    }

    placeText(str, path) {
        const text = new PointText(path.getPointAt(this.path.length / 2));
        text.fillColor = 'black';
        text.content = str;

        switch (this.direction) {
            case Direction.Forward:
                text.justification = 'center';
                break;
            case Direction.Right:
                text.justification = 'left';
                break;
            case Direction.Backward:
                text.justification = 'center';
                text.position.y += 10;
                break;
            case Direction.Left:
                text.justification = 'right';
                break;
        }
    }

    findClosestWall(walls: Wall[]) {
        let closestDistance = Number.MAX_SAFE_INTEGER;
        let closestWall = null;

        walls.forEach(wall => {
            const xDistance = this.center.x + wall.center.x;
            const yDistance = this.center.y + wall.center.y;
            const distance = this.pythagorasTheorem(xDistance, yDistance);

            if (distance < closestDistance) {
                closestDistance = distance;
                closestWall = wall;
            }
        });

        return closestWall;
    }

    pythagorasTheorem(a: number, b: number) {
        return Math.sqrt((a * a) + (b * b));
    }
}

export enum Direction {
    Forward,
    Right,
    Backward,
    Left
}
