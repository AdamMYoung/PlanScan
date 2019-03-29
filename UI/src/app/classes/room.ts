import { Coordinate } from './coordinate';
import { Path, Point, PointText } from 'paper';
import { Wall } from './wall';

export class Room {
    walls: Wall[];

    constructor(walls: Wall[]) {
        this.walls = walls;
    }

    draw() {
        this.walls.forEach(wall => {
            wall.draw();
        });
    }
}
