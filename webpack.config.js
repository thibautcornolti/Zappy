const path = require('path');

module.exports = {
    entry: [
        './src/main.ts',
        // './OBJLoader.js',
        // './MTLLoader.js'
    ],
    module: {
        rules: [
            {
                test: /\.tsx?$/,
                include: path.join(__dirname, 'src'),
                exclude: /node_modules/,
                loader: 'ts-loader'
            }
        ]
    },
    resolve: {
        extensions: ['.ts']
    },
    mode: 'development',
    output: {
        path: path.resolve(__dirname, 'public/dist'),
        filename: 'bundle.js'
    }
};