import { Measurement } from './../../classes/measurement';
import { Direction } from './../../classes/wall';
import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { PaperScope, Project, Path, Point, view, Matrix, Rectangle, Size } from 'paper';
import { Coordinate } from 'src/app/classes/coordinate';
import { Wall } from 'src/app/classes/wall';
import { Room } from '../../classes/room';
import { PositionalEntry } from '../../classes/positionalEntry';
import { JsonObject, JsonProperty, JsonConvert } from 'json2typescript';
import { Direct } from 'protractor/built/driverProviders';

@Component({
  selector: 'app-paper-canvas',
  templateUrl: './paper-canvas.component.html',
  styleUrls: ['./paper-canvas.component.css']
})
export class PaperCanvasComponent implements OnInit {
  @ViewChild('canvasElement') canvasElement: ElementRef;
  scope: PaperScope;
  project: Project;
  file: File;

  constructor() { }

  ngOnInit() {
    this.scope = new PaperScope();
    this.project = new Project(this.canvasElement.nativeElement);

    /*
    // Mark center
    const centerMarker = new Path.Rectangle(
      new Rectangle(new Point(this.scope.view.center.x - 1, this.scope.view.center.x - 1), new Size(2, 2))
    );
    centerMarker.fillColor = 'red';
    */

    /*
    // Draw walls
    const origin = new Coordinate(this.scope.view.center.x, this.scope.view.center.y);
    const walls = [
      new Wall(origin, Direction.Forward, [30, 30]),
      new Wall(origin, Direction.Backward, [20, 20]),
      new Wall(origin, Direction.Right, [20, 20]),
      new Wall(origin, Direction.Left, [20, 20]),
    ];

    const room = new Room(walls);
    room.draw();
    room.detectIntersections();
    */
  }

  fileChange(event: EventTarget) {
    const eventObj: MSInputMethodContext = <MSInputMethodContext>event;
    const target: HTMLInputElement = <HTMLInputElement>eventObj.target;
    const files: FileList = target.files;
    this.file = files[0];

    const reader = new FileReader();
    reader.onload = (e) => {
      const contents = (<any>e).target.result;

      const jsonConvert: JsonConvert = new JsonConvert();
      const entries = JSON.parse(contents);

      const viewCenter = new Coordinate(this.scope.view.center.x, this.scope.view.center.y);
      console.log(viewCenter);

      entries.forEach(entry => {
        const reading: PositionalEntry = jsonConvert.deserializeObject(entry, PositionalEntry);
        console.log(reading);

        // tslint:disable-next-line:max-line-length
        const origin = new Coordinate(viewCenter.x + this.scaleEncoderValue(reading.encoderXPos), viewCenter.y - this.scaleEncoderValue(reading.encoderYPos));
        const originMarker = new Path.Rectangle(
          new Rectangle(new Point(origin.x - 1, origin.x - 1), new Size(2, 2))
        );
        originMarker.fillColor = 'red';

        console.log(origin);

        const walls = [];

        reading.measurements.forEach(measurement => {
          let direction;
          switch (measurement.direction) {
            case 'Forward':
              direction = Direction.Forward;
              break;
            case 'Back':
              direction = Direction.Backward;
              break;
            case 'Left':
              direction = Direction.Left;
              break;
            case 'Right':
              direction = Direction.Right;
              break;
            default:
              return;
          }

          if (measurement.leftSensor === 0 && measurement.rightSensor === 0) {
            console.log('No sensor readings!');
            return;
          } else if (measurement.leftSensor === 0) {
            console.log('No left sensor reading!');
            measurement.leftSensor = measurement.rightSensor;
          } else if (measurement.rightSensor === 0) {
            console.log('No right sensor reading!');
            measurement.rightSensor = measurement.leftSensor;
          }

          const wall = new Wall(origin, direction, [measurement.leftSensor, measurement.rightSensor]);
          console.log(wall);
          walls.push(wall);
        });

        const room = new Room(walls);
        room.draw();
        room.detectIntersections();



      });
    };
    reader.readAsText(this.file);
  }

  scaleEncoderValue(number) {
    return ((number / 100) * 250);
  }
}
