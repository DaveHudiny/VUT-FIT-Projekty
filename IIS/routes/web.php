<?php

use Illuminate\Support\Facades\Route;

use App\Models\User;
use App\Http\Controllers\UserChangesController;
use App\Http\Controllers\AuctionRequestController;
use App\Http\Controllers\AuctionController;
use App\Models\Auction;
use Carbon\Carbon;
use App\Http\Controllers\LicitpageController;
use Illuminate\Support\Facades\Auth;

/*
|--------------------------------------------------------------------------
| Web Routes
|--------------------------------------------------------------------------
|
| Here is where you can register web routes for your application. These
| routes are loaded by the RouteServiceProvider within a group which
| contains the "web" middleware group. Now create something great!
|
*/

Route::get('/', [AuctionController::class, 'index']);

Route::get('/dashboard', function () {
    return view('dashboard');
})->middleware(['auth'])->name('dashboard');

Route::get('/admin/users', function(){
    return view('users',[ 'users' => User::all()]);
})->middleware('admin');

Route::post("/admin/users", [UserChangesController::class, 'edit'])->middleware('admin');

Route::get('/licitpage', function(){
    return view('licitpage', ['auctionUnapproved' => Auction::where([['approved', false], ['seller_id', '!=', Auth::user()->id]])->get(),
    'auctionApproved' => Auction::where([['approved', true],['date', '>=', Carbon::now()->toDateString()],
        ['licitator_id', '=', Auth::user()->id]])->get(),
    'auctionFinished' => Auction::where([['approved', true],['date', '<', Carbon::now()->toDateString()], 
        ['confirmed', false],['licitator_id', '=', Auth::user()->id]])->get(),
    'userRequests' => User::all()]);

    //return view('licitpage', ['auctionsToApprove' => Auction::where('approved', '=', false), 
    //'auctionsActive' => Auction::where(['approved', '=', true], ['date', '<=', Carbon::today()->toDateString()]),
    //'auctionsToFinish' => Auction::where(['approved', '=', true], ['date', '<=', Carbon::today()->toDateString()])]);
})->middleware('licitator');

Route::post('/licitpage', [LicitpageController::class, 'action']);

Route::get('/auction/request', function(){
    return view('auctionrequest');
})->middleware('auth');
Route::post('/auction/request', [AuctionRequestController::class, 'request'])->middleware('auth');
Route::post('/auction/{id}', [AuctionRequestController::class, 'requestBid']);

Route::get('/auction/{id}', function($slug)
{
    return view('auction', ['auction' => Auction::findOrFail($slug)]);
});

Route::get('/users/{id}/auctions', function($slug){
    if($slug != Auth::user()->id)
    {
        abort(403);
    }
    return view('myauctions');
})->middleware('auth');

Route::get('/users/{id}', function($slug)
{
    return view('user', ['user' => User::findOrFail($slug)]);
});

require __DIR__.'/auth.php';
