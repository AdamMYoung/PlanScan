import { Direction } from './../../classes/wall';
import { Component, OnInit, ViewChild, ElementRef } from '@angular/core';
import { PaperScope, Project, Path, Point, view, Matrix, Rectangle, Size } from 'paper';
import { Coordinate } from 'src/app/classes/coordinate';
import { Wall } from 'src/app/classes/wall';
import { Room } from '../../classes/room';
import { PositionalEntry } from '../../classes/positionalEntry';
import {JsonObject, JsonProperty, JsonConvert} from "json2typescript";

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

    // Mark center
    const centerMarker = new Path.Rectangle(
      new Rectangle(new Point(this.scope.view.center.x - 1, this.scope.view.center.x - 1), new Size(2, 2))
    );
    centerMarker.fillColor = 'red';

    // Draw walls
    const origin = new Coordinate(this.scope.view.center.x, this.scope.view.center.y);
    const walls = [
      new Wall(origin, Direction.Forward, [20, 20]),
      new Wall(origin, Direction.Backward, [20, 20]),
      new Wall(origin, Direction.Right, [20, 20]),
      new Wall(origin, Direction.Left, [20, 20]),
    ];

    const room = new Room(walls);
    room.draw();
    room.detectIntersections();
    
  }

  fileChange(event: EventTarget) {
    let eventObj: MSInputMethodContext = <MSInputMethodContext> event;
    let target: HTMLInputElement = <HTMLInputElement> eventObj.target;
    let files: FileList = target.files;
    this.file = files[0];
    
    var reader = new FileReader();
    reader.onload = function(e) {
      var contents = e.target.result;

      let jsonConvert: JsonConvert = new JsonConvert();
      var entries = JSON.parse(contents);

      entries.forEach(entry => {
        let object: PositionalEntry = jsonConvert.deserializeObject(entry, PositionalEntry);
        console.log(object);
      });
    };

    reader.readAsText(this.file);   
  }

}
