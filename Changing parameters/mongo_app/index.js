const express = require('express');
const bodyParser = require('body-parser');
const mongoose = require('mongoose');
const cors = require('cors');
const app = express();

app.set('view engine', 'ejs');
require('dotenv').config();

app.use(cors());

mongoose
  .connect(process.env.mongoString, { useNewUrlParser: true, useUnifiedTopology: true })
  .then(() => console.log('MongoDB connected...'))
  .catch(err => console.error('Could not connect to MongoDB...', err));

const SensorData = mongoose.model('SensorData', new mongoose.Schema({
  lightSensorDelay: Number,
  relayDelay: Number,
  pwmDelay: Number,
  standbyDelay: Number,
  timestamp: { type: Date, default: Date.now },
}));

app.use(bodyParser.urlencoded({ extended: true }));
app.use('/', require('./router')(SensorData));

const port = 3000;
const api = 'Add your IP Address';

app.listen(port, api, () => console.log(`Server running at: ${api}:${port}`));
