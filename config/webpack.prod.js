const MiniCssExtractPlugin = require('mini-css-extract-plugin')
const CssMinimizerPlugin = require('css-minimizer-webpack-plugin')
const webpack = require('webpack')
const { merge } = require('webpack-merge')

const paths = require('./paths')
const common = require('./webpack.common.js')
const vars = require('./vars');

module.exports = merge(common, {
  mode: 'production',
  devtool: false,
  output: {
    path: paths.build,
    publicPath: 'https://oriamn.github.io/chicken-liberator/',
    filename: 'js/[name].bundle.js',
  },
  module: {
    rules: [
      {
        test: /\.(scss|css)$/,
        use: [
          MiniCssExtractPlugin.loader,
          {
            loader: 'css-loader',
            options: {
              importLoaders: 2,
              sourceMap: false,
              modules: false,
            },
          },
          'postcss-loader',
          'sass-loader',
        ],
      },
    ],
  },
  plugins: [
    // Extracts CSS into separate files
    new MiniCssExtractPlugin({
      filename: 'styles/[name].css',
      chunkFilename: '[id].css',
    }),
    new webpack.DefinePlugin({
      ...(Object.keys(vars.production).reduce((acc,key)=>{
        acc[key]=JSON.stringify(vars.production[key])
        return acc
      }, {}))
    })
  ],
  optimization: {
    minimize: true,
    minimizer: [new CssMinimizerPlugin(), '...'],
    runtimeChunk: {
      name: 'runtime',
    },
  },
  performance: {
    hints: false,
    maxEntrypointSize: 512000,
    maxAssetSize: 512000,
  },
})
