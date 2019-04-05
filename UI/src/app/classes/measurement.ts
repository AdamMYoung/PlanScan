import {JsonObject, JsonProperty} from "json2typescript";

@JsonObject("Measurement")
export class Measurement {
    @JsonProperty("AngleHorizontal", Number)
    angleHorizontal: number = undefined;
    @JsonProperty("AngleVertical", Number)
    angleVertical: number = undefined;
    @JsonProperty("Direction", String)
    direction: string = undefined;
    @JsonProperty("LeftSensor", Number)
    leftSensor: number = undefined;
    @JsonProperty("RightSensor", Number)
    rightSensor: number = undefined;
}