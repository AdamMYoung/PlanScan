import { Coordinate } from './coordinate';
import { Path, Point } from 'paper';

export class Wall {
    path: Path;
    coordinates: Array<Coordinate> = [];

    constructor(origin: Coordinate, angle: number, distances: number[]) {
        const leftSensorDisplacementX = -5;
        const rightSensorDisplacementX = 5;

        const leftSensorOrigin = new Coordinate(origin.x + leftSensorDisplacementX, origin.y);
        const rightSensorOrigin = new Coordinate(origin.x + rightSensorDisplacementX, origin.y);

        this.coordinates.push(this.calculateCoordinate(leftSensorOrigin, angle, distances[0]));
        this.coordinates.push(this.calculateCoordinate(rightSensorOrigin, angle, distances[1]));
    }

    calculateCoordinate(origin: Coordinate, angle: number, distance: number): Coordinate {
        // Convert angle degrees to radians
        const radians = angle * (Math.PI / 180);

        return new Coordinate(
            (origin.x + distance * Math.sin(radians)),
            (origin.y + distance * Math.cos(radians))
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
