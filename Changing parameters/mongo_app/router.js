const express = require('express');
const router = express.Router();

module.exports = function (SensorData) {
       router.delete('/delete_last', async function (req, res) {
        try {
            const lastEntry = await SensorData.findOne().sort({ 'timestamp': -1 });
            if (!lastEntry) {
                return res.status(404).json({ status: 'No data found' });
            }

            await SensorData.deleteOne({ _id: lastEntry._id });
            res.json({ status: 'Success' });
        } catch (error) {
            console.error('Error deleting data:', error);
            res.status(500).json({ status: 'Error deleting data' });
        }
    });

    router.get('/get_recent_json', async function (req, res) {
        try {
            const data = await SensorData.findOne().sort({ 'timestamp': -1 });

            res.json({
                lightSensorDelay: data.lightSensorDelay,
                relayDelay: data.relayDelay,
                pwmDelay: data.pwmDelay,
                standbyDelay: data.standbyDelay
            });
        } catch (error) {
            console.error('Error fetching data:', error);
            res.status(500).json({ status: 'Error fetching data' });
        }
    });

    router.get('/get_recent', async function (req, res) {
        try {
            const data = await SensorData.findOne().sort({ 'timestamp': -1 });

            res.render('data', {
                lightSensorDelay: data.lightSensorDelay,
                relayDelay: data.relayDelay,
                pwmDelay: data.pwmDelay,
                standbyDelay: data.standbyDelay
            });
        } catch (error) {
            console.error('Error fetching data:', error);
            res.status(500).json({ status: 'Error fetching data' });
        }
    });

    // Define POST endpoint to receive sensor data
    router.post('/post_values', async function (req, res) {
        const { slider1, slider2, slider3, slider4 } = req.body;
        console.log('Data received:', req.body);

        // Create new entry
        const newEntry = new SensorData({
            lightSensorDelay: parseInt(slider1),
            relayDelay: parseInt(slider2),
            pwmDelay: parseInt(slider3),
            standbyDelay: parseInt(slider4),
        });

        // Save the entry
        try {
            const savedEntry = await newEntry.save();
            console.log('Data saved to MongoDB:', savedEntry);
            res.status(200).json({ status: 'Success' });
        } catch (error) {
            console.error('Error saving data:', error);
            res.status(500).json({ status: 'Error saving data' });
        }
    });

    return router;
};
