import {JsonObject, JsonProperty} from "json2typescript";
import { Measurement } from './measurement';
 
@JsonObject("PositionalEntry")
export class PositionalEntry {
    @JsonProperty("EncoderXPos", Number)
    encoderXPos: number = undefined;
    @JsonProperty("EncoderYPos", Number)
    encoderYPos: number = undefined;
    @JsonProperty("Measurements", [Measurement])
    measurements: Measurement[] = undefined;
}

