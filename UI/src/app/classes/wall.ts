import { Coordinate } from './coordinate';
import { Path, Point } from 'paper';

export class Wall {
    path: Path;
    coordinates: Array<Coordinate> = [];

    constructor(origin: Coordinate, direction: Direction, distances: number[]) {

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

        console.log(leftSensorOrigin);
        console.log(rightSensorOrigin);

        this.coordinates.push(this.calculateCoordinate(leftSensorOrigin, angle, distances[0]));
        this.coordinates.push(this.calculateCoordinate(rightSensorOrigin, angle, distances[1]));
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
}

export enum Direction {
    Forward,
    Right,
    Backward,
    Left
}
